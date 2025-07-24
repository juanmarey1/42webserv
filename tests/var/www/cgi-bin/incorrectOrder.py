#!/usr/bin/env python3

import os
import sys

# Read the request body (stdin)
body = sys.stdin.read()

# Output headers + blank line + body
print(f"Hello from CGI script!\nRequest body was:\n{body}")
print()
print("Content-Type: text/plain")