#Set-up
from __future__ import division
from visual import *
from visual.filedialog import get_file

scene = display(title = "PY5: Planetary Motion", x = 0, y = 1080,
                width = 960, height = 1080, background = color.black)
scene.range = (3e11, 3e11, 3e11)

#Creation Statements
earth = sphere(pos = (0, 0, 0), radius = 2e10, color = color.blue)
moon = sphere(pos = (1.5e11, 0, 0), radius = 8e9, color = color.white)
moon.trail = curve(color = color.green)

with open('MoonPos.txt', 'r') as fd:
    for line in fd:
        moon.pos.x = (longdouble(line.split()[0]))
        moon.pos.y = (longdouble(line.split()[1]))
        moon.pos.z = (longdouble(line.split()[2]))
        moon.trail.append(pos = moon.pos)
fd.close()
