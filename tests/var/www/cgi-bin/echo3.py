#!/usr/bin/env python3
import os
import sys

print("Content-Type: text/plain")
print()

print("Method:", os.environ.get("REQUEST_METHOD"))
print("Query:", os.environ.get("QUERY_STRING"))
print("Length:", os.environ.get("CONTENT_LENGTH"))
print("Type:", os.environ.get("CONTENT_TYPE"))

print("Body:")
print(sys.stdin.read())