# Official evaluator reports

The evaluator was run unchanged from upstream commit `8804159cef86ac7a578e611c6179b3859df0363f`. [Provenance and SHA-256 hashes](provenance.json) identify the evaluated source and exact report files.

- [Original baseline Markdown](initial/evaluation_report.md): 27 projects passed, 190/190 checks, overall FAILED because CAT14/CAT15 were missing. [Original exception log](initial/evaluator.log): PDF generation failed on an SVG. No original PDF was generated.
- [Corrected local-checkout PDF](local/evaluation_report.pdf) and [Markdown](local/evaluation_report.md).
- **Final public GitHub evaluation: [PDF](final/evaluation_report.pdf), [Markdown](final/evaluation_report.md), [CLI log](final/evaluator.log)**: 29 projects passed, 0 failed, 206/206 checks, overall PASSED.

The final CLI run cloned the pushed code commit `6428efcf8f32b2e0f7d26fd99935c4e0f61a7748` from the public target. The following evidence-only commit adds these reports and final documentation; no firmware or project configuration changed. The final branch is also rerun after this evidence commit.

Reports are retained exactly as generated. Their Markdown includes task-relative links copied from task READMEs; those links resolve relative to the task, not this report folder. Use the repository task README or the PDF to view embedded diagrams. The PDF includes the actual demo-labeled Node-RED capture.

An evaluator PASS checks required CAT discovery, project file existence and readable README availability. It does not establish successful physical experiments, correct wiring or Keil builds. Separate SDCC/GNU Arm builds passed for all 29 projects; hardware remains unverified.
