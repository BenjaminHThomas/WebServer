GET http://localhost:7777/timeout.py
HTTP 502
[Asserts]
body contains "502 Bad Gateway"

GET http://localhost:7777/timeout.php
HTTP 502
[Asserts]
body contains "502 Bad Gateway"