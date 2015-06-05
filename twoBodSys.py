from __future__ import division
from visual import *

scene = display(title = "Simple Two Body System", x = 0, y = 1080,
                width = 960, height = 1080, background = color.black)
scene.range = (3e11, 3e11, 3e11)

#Creation Statements
sun = sphere(pos = (0, 0, 0), radius = 2e10, color = color.yellow)
earth = sphere(pos = (1.5e11, 0, 0), radius = 8e9, color = color.blue)
earth.trail = curve(color = color.green)
sun.trail = curve(color = color.red)
f = arrow(pos = earth.pos, axis = (0, 0, 0), length = 0, color = color.red)
p = arrow(pos = earth.pos, axis = (0, 0, 0), length = 0, color = color.orange)

#Initial Conditions and Constants
MS = 2e30 #unit: kg
ME = 6e24 #unit: kg
G = 6.7e-11 #unit: N*m^2/kg^2

#initial net force
fnet = ((-1*G*ME*MS)/((mag(earth.pos-sun.pos))**2))*((earth.pos-sun.pos)/mag(earth.pos-sun.pos)) #unit: N

#initial momentum
earth.p = ME*vector(0, 3e4, 0) #unit: kg*m/s
sun.p = MS*vector(0, 5e3, 0)

t = 0 #unit: s
dt = 86400 #unit: s

#Update Statements
while t >= 0:
    rate(100)
    fnet = ((-1*G*ME*MS)/((mag(earth.pos-sun.pos))**2))*((earth.pos-sun.pos)/mag(earth.pos-sun.pos))
    earth.p += fnet * dt
    sun.p += fnet * dt
    earth.pos += (earth.p/ME) * dt
    sun.pos += (sun.p/MS) * dt
    
    p.pos = earth.pos
    f.pos = earth.pos
    p.axis = earth.p * 4e-19
    f.axis = fnet * 2e-12
    
    earth.trail.append(pos = earth.pos)
    sun.trail.append(pos = sun.pos)
    print("Pos:", earth.pos)
    
    t += dt

