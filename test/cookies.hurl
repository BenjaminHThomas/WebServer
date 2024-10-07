GET http://localhost:8885
HTTP 200
[Asserts]
body contains "Username: <input type=\"text\" name=\"username\"><br>"

POST http://localhost:8885
[FormParams]
username: hello world
email: hahalmao@haha.lmao

HTTP 200
[Asserts]
body contains "<p>Your email is: hahalmao@haha.lmao</p>"

GET http://localhost:8885
[Cookies]
username: trying_from_cookies
email: averygoodemail@email.com

HTTP 200
[Asserts]
body contains "<h1>Welcome, trying_from_cookies!</h1>"


DELETE http://localhost:8885
[Cookies]
username: trying_from_cookies
email: averygoodemail@email.com

HTTP 200
# [Asserts]
# cookie "username[Value]" == ""
# cookie "email[Value]" == ""