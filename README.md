# Hashcheck

Hashcheck is a C++17 command-line wrapper for Hashcat. It recognizes selected structured hash encodings and invokes Hashcat with a configured wordlist. It **cannot** uniquely identify most raw hexadecimal digests. Only run password auditing against data you are authorized to test.

## Build

On Debian/Ubuntu:

```sh
sudo apt-get update
sudo apt-get install -y g++ make rapidjson-dev
make clean && make check
```

`make check` builds the program, runs C++ detector tests and C++17 integration tests with a C++ mock Hashcat executable; it does **not** require a GPU or a real Hashcat installation. For actual use, install Hashcat separately and provide an authorized wordlist. On Fedora, install `gcc-c++ make rapidjson-devel` and run `make check`. The build and integration tests are primarily targeted at Linux; Windows and macOS builds have not been validated.

## Configuration

Create `config.json` (standard JSON; no comments):

```json
{
  "hashcat_path": "hashcat",
  "wordlist": "/absolute/path/to/authorized-wordlist.txt"
}
```

The wordlist must exist and be a regular file. `hashcat_path` may be a command on PATH or an absolute executable path. The included `src/config.json` is a placeholder, not a ready-to-run configuration.

## Usage

```sh
./bin/hashcheck-rc --help
./bin/hashcheck-rc -i '5d41402abc4b2a76b9719d911017c592' -c config.json --mode 0 --dry-run
./bin/hashcheck-rc -f hash.txt -c config.json --mode 1400
```

`-i HASH` accepts a single hash, `-f FILE` reads exactly one whitespace-delimited hash from a file, `-c CONFIG` selects configuration, `--mode NUMBER` specifies a Hashcat mode, and `--dry-run` prints the planned arguments without launching Hashcat. `-h`/`--help` displays usage. Duplicate `-c` or `--mode` arguments are errors.

Supported automatic identification: bcrypt (`3200`), MD5-crypt (`500`), SHA256-crypt (`7400`), and SHA512-crypt (`1800`). Raw hexadecimal digests are ambiguous and require an explicit mode. An explicit mode does not prove the hash format is correct. Hashcat performs the recovery attempt, not Hashcheck. Nonzero Hashcat exit codes (including exhausted and aborted) are surfaced as nonzero program exits; do not interpret a zero exit as proof of password recovery without checking Hashcat output.

## Security limitations

Hashcat receives arguments directly, not through a shell. The supplied hash is still visible in command-line process listings and dry-run output. Do not use Hashcheck for sensitive hashes on multi-user hosts without understanding this exposure. Hashes starting with `-`, containing NUL/newlines, or exceeding 16 KiB are rejected. This project does not implement a GUI, batch processing, or automatic verification of recovered passwords.

## Packaging and CI

`.github/workflows/build.yml` runs regression tests. `.github/workflows/release.yml` builds a Debian package artifact after testing; it **does not publish a GitHub release or update a signed APT repository**. Previous APT/RPM publication workflows are retained under `docs/legacy-workflows/` for reference only; they are intentionally disabled pending a secure packaging redesign. Package installation and Windows/macOS support still require validation.

See `DEVELOPMENT_STATUS.md` for verification limitations and remaining tasks. License: MIT (`LICENSE`).
