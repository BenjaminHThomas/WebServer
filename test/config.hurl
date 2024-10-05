GET http://localhost:8080
HTTP 200
[Asserts]
body contains "<p>Hello there! We're glad you're here.</p>"

GET http://localhost:8081
HTTP 200
[Asserts]
body contains "<h1>THIS IS THE ABOUT PAGE!</h1>"

GET http://localhost:9999
HTTP 200
[Asserts]
body contains "<p>another image, smaller this time, this should work too</p>"

#############################################################################
############################### ERROR PAGES #################################
#############################################################################

GET http://localhost:8080/not_existing
HTTP 404
[Asserts]
body contains "<p>Oops! The page you are looking for could not be found.</p>"

UPDATE http://localhost:8080/
HTTP 405
[Asserts]
body contains "<p>Sorry, the method you are using is not allowed on this server.</p>"


#############################################################################
############################### CGI HANDLING ################################
#############################################################################

GET http://localhost:8080/first/hello.php
HTTP 200
[Asserts]
body contains "<h1>Hello World from PHP CGI!</h1>"

GET http://localhost:9999/hello.py
HTTP 200
[Asserts]
body contains "<p>This is a simple Python CGI script.</p>"
