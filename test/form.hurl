GET http://localhost:8082
HTTP 200
[Asserts]
body contains "<form action=\"/process.py\" method=\"post\" enctype=\"multipart/form-data\">"


POST http://localhost:8082/process.py
[MultipartFormData]
name: some form name
project: a project name
description: a very detailed form description for my project

HTTP 200
[Asserts]
body contains "a very detailed form description for my project"
