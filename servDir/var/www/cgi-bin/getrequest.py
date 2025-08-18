#!/usr/bin/env python3
import os

print("Content-Type: text/plain")
print()
print("Hello from CGI script!")
print("Query string:", os.environ.get("QUERY_STRING"))