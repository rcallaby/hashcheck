# Development status

Date: 2026-11-16  
Revision: 2 (integration regression groundwork)

## Implemented

- Conservative structured-hash detection and explicit mode selection (revision 1).
- Direct Hashcat process invocation without a shell (revision 1).
- Black-box CLI integration tests with a mock Hashcat executable (revision 2).
- Unified root Makefile test entrypoint and delegated `src/Makefile` (revision 2).
- CI invokes `make check` with RapidJSON and Python 3 (revision 2).
- README testing instructions updated (revision 2).

## Verification performed

- GCC C++17 detector regression test: passed.
- Python integration suite syntax compilation: passed.
- Full CLI build/integration tests: **not run** (RapidJSON headers unavailable).
- Windows/macOS validation: **not run**.

## Remaining before release

1. Run `make check` on Linux with RapidJSON installed and correct any failures.
2. Validate process execution and test harness on Windows and macOS.
3. Audit APT/RPM publishing and release workflows and remove obsolete backups.
4. Expand negative tests, format fixtures, and Hashcat mode mapping verification.
5. Review Hashcat exit-code semantics and avoid conflating exhausted candidates with launch failure.
