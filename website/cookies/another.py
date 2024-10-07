#!/usr/bin/env python

import cgi
import cgitb
import os

cgitb.enable()

# Function to retrieve a cookie value by name
def get_cookie(name):
  if 'HTTP_COOKIE' in os.environ:
    cookies = os.environ['HTTP_COOKIE'].split(';')
    for cookie in cookies:
      cookie_name, cookie_value = cookie.strip().split('=', 1)
      if cookie_name == name:
        return cookie_value
  return None

# Get username and email from cookies
username = get_cookie('username')
email = get_cookie('email')

# Check if the request is a POST
if os.environ['REQUEST_METHOD'] == 'POST':
  # Parse the form data
  form = cgi.FieldStorage()
  username = form.getvalue('username')
  email = form.getvalue('email')

  # Set cookies in the response headers
  print("Set-Cookie: username={}; path=/".format(username))
  print("Set-Cookie: email={}; path=/".format(email))
  print("Content-type: text/html\n")
  print("<html><head><title>Cookies Set</title></head><body>")
  print("<h1>Cookies set successfully!</h1>")
  print("<p>Username: {}</p>".format(username))
  print("<p>Email: {}</p>".format(email))
  print("</body></html>")

# If not a POST request, check for existing cookies
elif username and email:
  print("Content-type: text/html\n")
  print("<html><head><title>Welcome</title></head><body>")
  print("<h1>Welcome back, {}!</h1>".format(username))
  print("<p>Your email is: {}</p>".format(email))
  print("</body></html>")

# If no cookies or POST request, display the form
else:
  print("Content-type: text/html\n")
  print("<html><head><title>Login</title></head><body>")
  print("<h1>Please login</h1>")
  print("<form method='POST' action='/'>")
  print("<label for='username'>Username:</label>")
  print("<input type='text' id='username' name='username' required><br><br>")
  print("<label for='email'>Email:</label>")
  print("<input type='email' id='email' name='email' required><br><br>")
  print("<input type='submit' value='Login'>")
  print("</form>")
  print("</body></html>")