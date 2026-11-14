#!/usr/bin/env python3
"""Local audit mirroring the report's file patterns, with integrity/link checks."""
from pathlib import Path
import json,re,hashlib,xml.etree.ElementTree as ET,urllib.parse,sys
ROOT=Path(__file__).resolve().parents[1]
TASKS=json.loads((ROOT/'docs/task_paths.json').read_text())
def ihex(path):
 try:
  lines=path.read_text().splitlines();end=False;data=False
  for line in lines:
   if not line.startswith(':'):return False
   record=bytes.fromhex(line[1:])
   if len(record)!=record[0]+5 or sum(record)%256 or end:return False
   if record[3]==0:data=True
   if record[3]==1:end=True
  return end and data
 except (ValueError,OSError):return False
def audit():
 builds={r['task']:r for r in json.loads((ROOT/'docs/build_results.json').read_text())}
 tasks=[];errors=[]
 for platform, cats in [('8051',range(2,8)),('STM32',range(8,16))]:
  for cat in cats:
   prefix=f'{platform}/CAT{cat}/'
   if not (ROOT/platform/f'CAT{cat}').is_dir() or not any(task.startswith(prefix) for task in TASKS):
    errors.append(prefix+': required CAT missing from filesystem or manifest')
 for task in TASKS:
  d=ROOT/task
  if task.startswith('8051'):
   checks={'Keil project':bool(list(d.glob('*.uvproj'))+list(d.glob('*.uvprojx'))),'C source':bool(list(d.glob('*.c'))),'STARTUP.A51':(d/'STARTUP.A51').is_file(),'Objects folder':(d/'Objects').is_dir(),'Objects HEX':bool(list((d/'Objects').glob('*.hex'))),'README':(d/'README.md').is_file()}
  else:
   checks={'ioc':bool(list(d.glob('*.ioc'))),'mxproject':(d/'.mxproject').is_file(),'Core':(d/'Core').is_dir(),'Drivers':(d/'Drivers').is_dir(),'MDK-ARM':(d/'MDK-ARM').is_dir(),'Keil project':bool(list((d/'MDK-ARM').glob('*.uvprojx'))),'startup':bool(list((d/'MDK-ARM').glob('startup_*.s'))),'README':(d/'README.md').is_file()}
  b=builds.get(task,{});source=d/('main.c' if task.startswith('8051') else 'Core/Src/main.c');artifact=ROOT/b.get('artifact','nonexistent')
  compiled=b.get('status')=='PASS' and source.is_file() and artifact.is_file()
  if compiled:compiled=hashlib.sha256(source.read_bytes()).hexdigest()==b.get('source_sha256') and hashlib.sha256(artifact.read_bytes()).hexdigest()==b.get('sha256') and ihex(artifact)
  if not compiled:errors.append(task+': missing/stale/invalid build evidence')
  if not all(checks.values()):errors.append(task+': report file check failed')
  for project in list(d.glob('*.uvproj'))+list(d.glob('*.uvprojx'))+list((d/'MDK-ARM').glob('*.uvprojx')):
   for node in ET.parse(project).findall('.//FilePath'):
    if node.text and not (project.parent/node.text.replace('\\','/')).is_file():errors.append(task+': missing Keil source '+node.text)
  tasks.append({'task':task,'checks':checks,'README':'PASS' if checks['README'] else 'FAIL','circuit':'PASS' if (d/'circuit.png').is_file() else 'FAIL','compilation':'PASS' if compiled else 'FAIL','hardware':'NOT VERIFIED','overall':'NOT VERIFIED' if compiled and all(checks.values()) else 'FAIL'})
 for p in ROOT.rglob('*.md'):
  if '.git' in p.parts or '.build' in p.parts:continue
  base=p.parent
  generated_report=p.name=='evaluation_report.md' and 'evaluation' in p.parts
  for line in p.read_text(errors='replace').splitlines():
   if generated_report and line.startswith(('## 8051/','## STM32/')):
    base=ROOT/line[3:].strip()
   for link in re.findall(r'!?\[[^\]]*\]\(([^\s)]+)(?:\s+"[^"]*")?\)',line):
    link=urllib.parse.unquote(link.split('#')[0])
    if not link or re.match(r'\w+://|mailto:',link):continue
    if not (base/link).exists():errors.append(str(p.relative_to(ROOT))+': broken local link '+link)
 # Deliberately scoped scan: known leaked credentials/other student IDs, plus common token formats.
 patterns=[r'\b25\d{9}\b',r'gh[pousr]_[A-Za-z0-9]{20,}',r'github_pat_[A-Za-z0-9_]{20,}',r'-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----']
 for p in ROOT.rglob('*'):
  if not p.is_file() or '.git' in p.parts or '.build' in p.parts or p.name=='validate.py':continue
  if p.suffix.lower() in ('.c','.h','.md','.json','.xml','.ioc','.uvproj','.uvprojx','.py','.js') or p.name=='.mxproject':
   text=p.read_text(errors='replace')
   # Provenance contains the source repository URL by design; skip the ID inside that URL.
   text=re.sub(r'https://github.com/[^\s)]+','',text)
   text=text.replace('25011102113','')
   if any(re.search(pattern,text) for pattern in patterns):errors.append(str(p.relative_to(ROOT))+': privacy/secret pattern needs review')
 results={'tasks':tasks,'tasks_count':len(tasks),'file_checks_passed':sum(sum(t['checks'].values()) for t in tasks),'file_checks_total':sum(len(t['checks']) for t in tasks),'errors':errors,'official_evaluator':'See docs/evaluation for original unmodified evaluator reports','hardware':'NOT VERIFIED','scope':'CAT2-CAT15; no hardware/simulator/Keil/CubeMX execution asserted'}
 (ROOT/'docs/validation_results.json').write_text(json.dumps(results,indent=2)+'\n')
 print(json.dumps({k:v for k,v in results.items() if k!='tasks'},indent=2))
 return results
if __name__=='__main__':sys.exit(bool(audit()['errors']))
