#Set-up
from __future__ import division
from visual import *
from visual.filedialog import get_file

scene = display(title = "SPICE - TBOS Visual", x = 480, y =120,
                width = 960, height = 700, background = color.black)
scene.range = (3e11, 3e11, 3e11)

#Creation Statements
earth = sphere(pos = (0, 0, 0), radius = 2e10, color = color.blue)
moon = sphere(pos = (1.5e11, 0, 0), radius = 8e9, color = color.white)
moon.trail = curve(color = color.green)
earth.trail = curve(color = color.blue)

with open('MoonPos.txt', 'r') as fd:
    for line in fd:
	moon.pos.x = (longdouble(line.split()[0]))
	moon.pos.y = (longdouble(line.split()[1]))
	moon.pos.z = (longdouble(line.split()[2]))
	
	earth.pos.x = (longdouble(line.split()[3]))
	earth.pos.y = (longdouble(line.split()[4]))
	earth.pos.z = (longdouble(line.split()[5]))
	
	moon.trail.append(pos=moon.pos)
	earth.trail.append(pos=earth.pos)
	
	scene.center=earth.pos

        #rate(1)

fd.close()
