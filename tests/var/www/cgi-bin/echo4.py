#!/usr/bin/env python3

import os
import sys

# CGI headers
print("Status: 201 Created")
print("Content-Type: text/plain")
print()

# Response body
print("Hello from CGI script!")
print("Request method was:", os.environ.get("REQUEST_METHOD"))