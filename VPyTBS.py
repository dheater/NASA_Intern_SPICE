#Set-up
from __future__ import division
from visual import *
from visual.filedialog import get_file

scene = display(title = "SPICE - TBOS Visual", x = 480, y =120,
                width = 960, height = 700, background = color.black)
scene.range = (1.4e7,1.4e7,1.4e7)

#Creation Statements
earth = sphere(pos = (0, 0, 0), radius = 933333, material = materials.earth)
moon = sphere(pos = (7e6, 0, 0), radius = 373333, material = materials.rough, color = color.white)
moon.trail = curve(color = color.green)
earth.trail = curve(color = color.blue)

#Open MoonPos.txt (output file for TBOS.c)
with open('MoonPos.txt', 'r') as fd:
    for line in fd:
	#Reads in the position vectors for earth/moon
	moon.pos.x = (longdouble(line.split()[0]))
	moon.pos.y = (longdouble(line.split()[1]))
	moon.pos.z = (longdouble(line.split()[2]))
	
	earth.pos.x = (longdouble(line.split()[3]))
	earth.pos.y = (longdouble(line.split()[4]))
	earth.pos.z = (longdouble(line.split()[5]))
	
	moon.trail.append(pos=moon.pos)
	earth.trail.append(pos=earth.pos)
	
	#Explosion
	if mag(earth.pos-moon.pos) <= earth.radius+moon.radius:
		earth.color=color.red
		moon.color=color.red
		print '\a'
		break
	
	scene.center=earth.pos

#t = 0

#expl = text(text='Boom!!!', pos=((moon.pos-earth.pos)/mag(moon.pos-earth.pos))*moon.radius, height = 2e9, color = color.orange, depth = 2e8)

#while true:
#	t+=1
#	if t<2000:
#		expl.radius=expl.radius*1.1
#	else:
#		expl.radius=expl.radius*1.001
#	rate(1000)
	


fd.close()
