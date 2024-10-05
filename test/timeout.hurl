GET http://localhost:9999/timeout.py
HTTP 502
[Asserts]
body contains "502 Bad Gateway"

GET http://localhost:9999/timeout.php
HTTP 502
[Asserts]
body contains "502 Bad Gateway"