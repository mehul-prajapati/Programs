#!/usr/bin/env python

# Author: Mehul Prajapati
# Date: 14/03/2015

# Python libs
import sys
import getpass

# import the selenium libs
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select

# Get input from user
login = raw_input("Enter your user name: ")
pw = getpass.getpass()

# Create instance of Firefox
browser = webdriver.Firefox()
browser.get("https://192.168.2.10:2000")

element = browser.find_element_by_id("username")
element.send_keys(login)

element = browser.find_element_by_name("password")
element.send_keys(pw)

# Login
browser.find_element_by_name("login").click()
browser.find_element_by_link_text("Projects").click()
browser.find_element_by_link_text("Extra Time").click()
browser.find_element_by_link_text('Log time').click()

element = browser.find_element_by_id("time_entry_issue_id")
element.send_keys("1169")

start=int(sys.argv[1])
end=int(sys.argv[2]) + 1

month=sys.argv[3]

# from start date to end date
for i in range (start,end):
	element = browser.find_element_by_id("time_entry_spent_on")
	element.clear()
	if i < 10:
		if int(month) < 10:
			element.send_keys("2015-0"+month+"-0"+str(i))
		else:
			element.send_keys("2015-"+month+"-0"+str(i))
	else:
		if int(month) < 10:
			element.send_keys("2015-0"+month+"-"+str(i))
		else:
			element.send_keys("2015-"+month+"-"+str(i))

	element = browser.find_element_by_id("time_entry_hours")
	element.send_keys("1")

	select = Select(browser.find_element_by_id("time_entry_activity_id"))
	select.select_by_visible_text("Other")

	browser.find_element_by_name("continue").click()

