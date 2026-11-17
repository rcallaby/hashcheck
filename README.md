# Hashcheck

Hashcheck is a C++17 command-line wrapper for Hashcat. It identifies a limited set of structured password-hash encodings, accepts an explicitly selected Hashcat mode, and launches Hashcat with an authorized wordlist. It does **not** reliably infer algorithms from raw hexadecimal digests: many hash algorithms produce identical-looking output.

## Requirements

- C++17 compiler and GNU Make
- RapidJSON development headers (for example `rapidjson-dev` on Debian-based Linux)
- Hashcat installed and accessible in `PATH`, or configured by absolute path
- A wordlist you are authorized to use

Build from the repository root with `make`. Run detector and mock-Hashcat integration regression tests with `make test` (requires Python 3). Run `make clean` to remove generated files.

## Configuration

Create a valid JSON file such as `config.json`:

```json
{
  "hashcat_path": "hashcat",
  "wordlist": "/path/to/authorized/wordlist.txt"
}
```

JSON comments are not supported. Paths containing spaces are supported without shell quoting. Arbitrary shell options are deliberately not supported.

## Usage

```sh
./bin/hashcheck-rc --help
./bin/hashcheck-rc -i '5d41402abc4b2a76b9719d911017c592' -c config.json --mode 0 --dry-run
./bin/hashcheck-rc -f hash.txt -c config.json --mode 1400
```

`-i` supplies one hash directly; `-f` reads the first whitespace-delimited token from a file. `-c` supplies configuration. `--mode` selects the Hashcat mode. `--dry-run` displays the planned argument vector without running Hashcat. Structured bcrypt, MD5-crypt, SHA256-crypt, and SHA512-crypt hashes may be recognized automatically. Unrecognized or ambiguous inputs require `--mode`; selecting a mode is the user's responsibility.

The application does not provide a graphical interface, interactive menu, hash validation against known plaintext, or guaranteed password recovery. It does not implement `-t`, `-w`, `-o`, or `--verbose`. The Hashcat executable performs the actual password recovery attempt. Hashcat failures are reported as errors.

## Safety and limitations

Use only with hashes and systems you are authorized to audit. A hash's representation is not proof of its algorithm. Dry-run output includes the supplied hash; avoid sharing it if sensitive. Hashcat's own version-specific input formats and mode documentation remain authoritative. This application invokes Hashcat without a shell to avoid shell metacharacter interpretation.

## Testing

`make test` builds the full application, runs detector regression tests, and runs black-box integration tests using a mock Hashcat executable. The tests require Python 3 and RapidJSON development headers but do not require Hashcat, a GPU, or actual password recovery. CI runs `make check` on pull requests. `make -C src` delegates to the root Makefile.

## License

MIT; see `LICENSE`.

## Development status (Revision 2, 2026-11-16)

The detector unit tests have been compiled and executed successfully with GCC/C++17.
The Python integration test suite passes syntax compilation, but the full CLI integration
suite has **not** been executed in this environment because RapidJSON headers are absent.
Run `make check` on a machine with RapidJSON installed before tagging a release.
The APT/RPM publishing and release workflows have not been audited in this revision.
