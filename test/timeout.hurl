GET http://localhost:7777/
HTTP 504
[Asserts]
body contains "504 Gateway Timeout"

GET http://localhost:7777/timeout.php
HTTP 504
[Asserts]
body contains "504 Gateway Timeout"