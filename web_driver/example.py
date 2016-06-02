#!/usr/bin/env python

# import the libs
from selenium import webdriver
from selenium.webdriver.common.keys import Keys

# Create instance of Firefox
browser = webdriver.Firefox()
browser.get("http://www.gmail.com")

element = browser.find_element_by_name("Email")
element.send_keys("mehulprajapati2802")

element = browser.find_element_by_name("Passwd")
element.send_keys("mathematician")

elem = browser.find_element_by_id('signIn')
elem.send_keys(Keys.RETURN)

