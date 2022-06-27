import imageio as iio
from datetime import date
import os
import time

today = date.today()

dateRightFormat = str(today).split("-")

day   = dateRightFormat[2]
month = dateRightFormat[1]
year  = dateRightFormat[0]

dateRightFormat[0] = month
dateRightFormat[1] = day
dateRightFormat[2] = year

newToday = "/".join(dateRightFormat)

finalDate = newToday[1]