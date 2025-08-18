#!/bin/bash

# Exit on error
set -e

TEST_WORDLIST="/usr/share/seclists/Passwords/Common-Credentials/10k-most-common.txt"
TEST_HASHFILE="testhashes.txt"

if [ ! -f "$TEST_HASHFILE" ]; then
  echo "❌ $TEST_HASHFILE not found!"
  exit 1
fi

if [ -f "$TEST_WORDLIST" ]; then
  echo "✅ Found seclists wordlist: $TEST_WORDLIST"
  echo "✅ Found test hashes: $TEST_HASHFILE"

  while IFS=: read -r HASH TYPE; do
    # skip empty/comment lines
    [[ -z "$HASH" || "$HASH" =~ ^# ]] && continue

    echo "🔍 Testing ${TYPE:-Unknown} hash: $HASH"
    ./src/hashcheck --wordlist "$TEST_WORDLIST" --hash "$HASH"
  done < "$TEST_HASHFILE"

else
  echo "❌ Seclists not found"
  exit 1
fi
