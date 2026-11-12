#!/usr/bin/env python3
"""Reproducible SDCC firmware builds and GNU ARM compile/link builds.
C51 extensions are translated mechanically for SDCC in ignored .build/ only.
The Keil projects remain C51/MDK projects and are not claimed tested by this script.
"""
import os,sys,re,json,shutil,subprocess,hashlib,xml.etree.ElementTree as ET
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
TASKS=json.loads((ROOT/'docs/task_paths.json').read_text())
def run(args,log):
 p=subprocess.run([str(a) for a in args],capture_output=True,text=True)
 log.append('$ '+' '.join(str(a).replace(str(ROOT),'<repo>') for a in args)+'\n'+p.stdout+p.stderr)
 if p.returncode:raise RuntimeError('command exited '+str(p.returncode))
def build(task):
 folder=ROOT/task;temp=ROOT/'.build'/task;temp.mkdir(parents=True,exist_ok=True)
 log=[];result={'task':task,'status':'FAIL','hardware':'NOT VERIFIED'}
 try:
  if task.startswith('8051'):
   sdcc=os.environ.get('SDCC','sdcc');prefix=os.environ.get('SDCC_HOME','')
   text=(folder/'main.c').read_text()
   text=re.sub(r'#include\s*<reg(?:51|52)\.h>','#include <8052.h>',text,flags=re.I)
   def sb(m):return '__sbit __at (0x%02X) %s;' % ({'P0':0x80,'P1':0x90,'P2':0xA0,'P3':0xB0}[m[2]]+int(m[3]),m[1])
   text=re.sub(r'sbit\s+(\w+)\s*=\s*(P[0-3])\s*\^\s*(\d)\s*;',sb,text)
   text=re.sub(r'sfr\s+(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;',r'__sfr __at (\2) \1;',text)
   text=re.sub(r'\bbit\b','__bit',text)
   text=re.sub(r'\bcode\b','__code',text)
   text=re.sub(r'\binterrupt\s+(\d+)',r'__interrupt (\1)',text)
   source=temp/'main.c';source.write_text(text)
   flags=['-mmcs51','--model-small','--code-size','8192','--iram-size','256']
   if prefix:flags+=['-I'+prefix+'/include','-L'+prefix+'/lib/small']
   run([sdcc,*flags,source,'-o',temp/'firmware.ihx'],log)
   dest=folder/'Objects'/('firmware.hex');dest.parent.mkdir(exist_ok=True);shutil.copy(temp/'firmware.ihx',dest)
   result.update(toolchain='SDCC 4.2.0; mechanically translated C51 syntax',artifact=str(dest.relative_to(ROOT)))
  else:
   gcc=os.environ.get('ARM_GCC','arm-none-eabi-gcc');objcopy=os.environ.get('ARM_OBJCOPY','arm-none-eabi-objcopy')
   project=next((folder/'MDK-ARM').glob('*.uvprojx'));tree=ET.parse(project)
   defines=next(x.text for x in tree.findall('.//Define') if x.text)
   f1='STM32F103' in defines;cpu='cortex-m3' if f1 else 'cortex-m4'
   flags=['-mcpu='+cpu,'-mthumb','-mfloat-abi=soft','-Os','-g','-ffunction-sections','-fdata-sections','-std=gnu11','-Wall','-Wextra']
   flags+=['-D'+x for x in defines.split(',')]
   for x in tree.findall('.//IncludePath'):
    for v in (x.text or '').split(';'):
     if v:flags+=['-I'+str((project.parent/v.replace('\\','/')).resolve())]
   if os.environ.get('NEWLIB_INCLUDE'):flags+=['-I'+os.environ['NEWLIB_INCLUDE']]
   files=[(project.parent/x.text.replace('\\','/')).resolve() for x in tree.findall('.//FilePath') if x.text and x.text.endswith('.c')]
   files+=list((folder/'Core/Src').glob('bme*.c'))
   files=list(dict.fromkeys(files));objects=[]
   for i,source in enumerate(files):
    obj=temp/(str(i)+'.o');run([gcc,*flags,'-c',source,'-o',obj],log);objects.append(obj)
   startup=folder/'GNU/startup.c';obj=temp/'startup.o';run([gcc,*flags,'-c',startup,'-o',obj],log);objects.append(obj)
   libs=['-L'+os.environ['NEWLIB_LIB']] if os.environ.get('NEWLIB_LIB') else []
   # Choose a portable soft-float ARMv7-M library for both supported cores.
   elf=temp/'firmware.elf'
   run([gcc,'-mcpu='+cpu,'-mthumb','-mfloat-abi=soft','-nostartfiles','-T',folder/'GNU/memory.ld','-Wl,--gc-sections',*objects,*libs,'-Wl,--start-group','-lc','-lnosys','-lgcc','-Wl,--end-group','-o',elf],log)
   dest=folder/'Objects/firmware.hex';dest.parent.mkdir(exist_ok=True)
   run([objcopy,'-O','ihex',elf,dest],log)
   result.update(toolchain='GNU Arm Embedded 12.2.1; soft-float',artifact=str(dest.relative_to(ROOT)))
  result['status']='PASS';result['sha256']=hashlib.sha256((ROOT/result['artifact']).read_bytes()).hexdigest()
 except (RuntimeError,OSError,StopIteration) as e:result['error']=str(e)
 result['source_sha256']=hashlib.sha256((folder/('main.c' if task.startswith('8051') else 'Core/Src/main.c')).read_bytes()).hexdigest()
 (temp/'build.log').write_text('\n'.join(log))
 return result
if __name__=='__main__':
 selected=[t for t in TASKS if len(sys.argv)==1 or any(t==a or t.startswith(a+'/') for a in sys.argv[1:])]
 results=[]
 for task in selected:
  r=build(task);results.append(r);print(task,r['status'],r.get('error',''),flush=True)
 previous=json.loads((ROOT/'docs/build_results.json').read_text()) if (ROOT/'docs/build_results.json').exists() else []
 merged={r['task']:r for r in previous}
 merged.update({r['task']:r for r in results})
 (ROOT/'docs/build_results.json').write_text(json.dumps([merged[t] for t in TASKS if t in merged],indent=2)+'\n')
 sys.exit(any(r['status']!='PASS' for r in results))
