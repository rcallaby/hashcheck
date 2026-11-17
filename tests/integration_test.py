#!/usr/bin/env python3
"""Black-box CLI regression tests; mock executable never attempts recovery."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

BINARY = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path('bin/hashcheck-rc').resolve()
sys.argv = sys.argv[:1]

class IntegrationTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix='hashcheck tests ')
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.log = self.root / 'argv.json'
        self.mock = self.root / 'mock hashcat.py'
        self.mock.write_text('#!' + sys.executable + '\nimport json,os,sys\n'
                             'with open(os.environ["HASHCHECK_TEST_LOG"],"w") as f: json.dump(sys.argv[1:],f)\n'
                             'sys.exit(int(os.environ.get("HASHCHECK_TEST_EXIT","0")))\n')
        self.mock.chmod(0o755)
        self.wordlist = self.root / 'word list;$(echo injected).txt'
        self.wordlist.write_text('test\n')
        self.config = self.root / 'config.json'
        self.config.write_text(json.dumps({'hashcat_path': str(self.mock), 'wordlist': str(self.wordlist)}))
        self.env = {**os.environ, 'HASHCHECK_TEST_LOG': str(self.log)}

    def run_cli(self, *args, env=None):
        return subprocess.run([str(BINARY), *args], text=True, capture_output=True,
                              env=env or self.env, timeout=10)

    def test_explicit_mode_passes_literal_arguments(self):
        hash_value = 'a' * 32
        result = self.run_cli('-i', hash_value, '-c', str(self.config), '--mode', '0')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(json.loads(self.log.read_text()),
                         ['-m', '0', '-a', '0', hash_value, str(self.wordlist)])

    def test_structured_hash_auto_mode(self):
        h = '$2b$12$' + 'a' * 53
        result = self.run_cli('-i', h, '-c', str(self.config))
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(json.loads(self.log.read_text())[1], '3200')

    def test_ambiguous_digest_requires_mode(self):
        result = self.run_cli('-i', 'a' * 64, '-c', str(self.config))
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('--mode', result.stderr)
        self.assertFalse(self.log.exists())

    def test_dry_run_never_executes(self):
        result = self.run_cli('-i', 'a' * 32, '-c', str(self.config), '--mode', '0', '--dry-run')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('Dry run:', result.stdout)
        self.assertFalse(self.log.exists())

    def test_file_input(self):
        p = self.root / 'input hash.txt'
        p.write_text('a' * 32 + '\n')
        result = self.run_cli('-f', str(p), '-c', str(self.config), '--mode', '0')
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_invalid_config_and_missing_file(self):
        self.config.write_text('{broken')
        result = self.run_cli('-i', 'a' * 32, '-c', str(self.config), '--mode', '0')
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('JSON parse error', result.stderr)
        result = self.run_cli('-f', str(self.root / 'missing'), '-c', str(self.config), '--mode', '0')
        self.assertNotEqual(result.returncode, 0)

    def test_invalid_config_shape(self):
        for obj in ([], {}, {'hashcat_path': 123, 'wordlist': 'x'},
                    {'hashcat_path': 'x', 'wordlist': ''}):
            with self.subTest(obj=obj):
                self.config.write_text(json.dumps(obj))
                result = self.run_cli('-i', 'a' * 32, '-c', str(self.config), '--mode', '0')
                self.assertNotEqual(result.returncode, 0)
                self.assertFalse(self.log.exists())

    def test_argument_errors(self):
        for args in (('--bad',), ('-i',), ('--mode', 'nope'),
                     ('-i', 'a', '-f', 'b'), ('-i', 'a', '-c', str(self.config), '--mode', '-1')):
            with self.subTest(args=args):
                self.assertNotEqual(self.run_cli(*args).returncode, 0)

    def test_mock_failure_propagates(self):
        env = {**self.env, 'HASHCHECK_TEST_EXIT': '42'}
        result = self.run_cli('-i', 'a' * 32, '-c', str(self.config), '--mode', '0', env=env)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('42', result.stderr)

    def test_multiple_hashes_in_file_rejected(self):
        p = self.root / 'multiple.txt'
        p.write_text('a' * 32 + '\n' + 'b' * 32 + '\n')
        result = self.run_cli('-f', str(p), '-c', str(self.config), '--mode', '0')
        self.assertNotEqual(result.returncode, 0)
        self.assertFalse(self.log.exists())

    def test_empty_hash_file_rejected(self):
        p = self.root / 'empty.txt'
        p.write_text('')
        result = self.run_cli('-f', str(p), '-c', str(self.config), '--mode', '0')
        self.assertNotEqual(result.returncode, 0)

    def test_missing_wordlist_rejected(self):
        self.wordlist.unlink()
        result = self.run_cli('-i', 'a' * 32, '-c', str(self.config), '--mode', '0')
        self.assertNotEqual(result.returncode, 0)
        self.assertFalse(self.log.exists())

    def test_duplicate_options_rejected(self):
        for extra in (('-c', str(self.config)), ('--mode', '0')):
            args = ('-i', 'a' * 32, '-c', str(self.config), '--mode', '0') + extra
            with self.subTest(extra=extra):
                self.assertNotEqual(self.run_cli(*args).returncode, 0)

    def test_hashcat_exit_statuses(self):
        for status in (1, 2, 42):
            with self.subTest(status=status):
                env = {**self.env, 'HASHCHECK_TEST_EXIT': str(status)}
                result = self.run_cli('-i', 'a' * 32, '-c', str(self.config), '--mode', '0', env=env)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(str(status), result.stderr)

    def test_no_shell_interpretation(self):
        marker = self.root / 'INJECTION_MARKER'
        evil_hash = '$(touch ' + str(marker) + ')'
        result = self.run_cli('-i', evil_hash, '-c', str(self.config), '--mode', '0')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertFalse(marker.exists())
        self.assertEqual(json.loads(self.log.read_text())[4], evil_hash)

if __name__ == '__main__':
    if not BINARY.is_file():
        raise SystemExit('Binary not found: ' + str(BINARY))
    unittest.main()
