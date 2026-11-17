# Development status — Revision 3 (2026-11-16)

## Implemented
- Conservative structured-hash detection and explicit Hashcat modes.
- Direct process invocation without shell interpretation.
- Single-hash input validation and stricter CLI argument handling.
- Wordlist path validation, clearer child exit status messages.
- Detector and mock-Hashcat regression tests, including negative cases.
- Consolidated Makefiles, README, man page, and CI test workflow.
- Disabled conflicting legacy APT/RPM publishing workflows; Debian artifact workflow is test-gated and does not publish releases.

## Verification
- C++ detector tests: run locally in this revision.
- Python integration test syntax: checked locally.
- Full application compilation/integration: **not verified** in this environment because RapidJSON development headers are absent.
- Debian packaging workflow: **not executed** on GitHub Actions.
- Windows/macOS: **not verified**.

## Release blockers
1. Run `make clean && make check` on Ubuntu with RapidJSON installed and inspect results.
2. Verify Hashcat invocation with an authorized test fixture and installed Hashcat.
3. Validate the generated .deb in a clean Ubuntu environment.
4. Implement and test a separate, signed APT repository publishing process if required.
5. Validate Windows/macOS builds before advertising cross-platform support.
6. Review command-line hash exposure and consider secure hash-file staging for sensitive use.
