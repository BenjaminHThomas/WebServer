#!/usr/bin/env python3

import cgi
import os
from http import cookies
import datetime

def print_headers(cookie=None):
    print("Content-Type: text/html")
    if cookie:
        print(cookie.output())
    print()  # blank line to end headers

def print_html(content):
    print("<html><body>")
    print(content)
    print("</body></html>")

def create_form():
    return """
    <form method="POST">
        Username: <input type="text" name="username"><br>
        Email: <input type="text" name="email"><br>
        <input type="submit" value="Submit">
    </form>
    """

def create_welcome_page(username, email):
    return f"""
    <h1>Welcome, {username}!</h1>
    <p>Your email is: {email}</p>
    <p>This information is stored in cookies.</p>
    """

def main():
    form = cgi.FieldStorage()
    cookie = cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))

    if "username" in cookie and "email" in cookie:
        # Cookies exist, show welcome page
        print_headers()
        print_html(create_welcome_page(cookie["username"].value, cookie["email"].value))
    elif "username" in form and "email" in form:
        # Form submitted, set cookies and show welcome page
        username = form["username"].value
        email = form["email"].value

        new_cookie = cookies.SimpleCookie()
        new_cookie["username"] = username
        new_cookie["email"] = email
        for morsel in new_cookie.values():
            morsel["expires"] = (datetime.datetime.now() + datetime.timedelta(days=30)).strftime("%a, %d-%b-%Y %H:%M:%S GMT")

        print_headers(new_cookie)
        print_html(create_welcome_page(username, email))
    else:
        # No cookies or form data, show the form
        print_headers()
        print_html(create_form())

if __name__ == "__main__":
    main()