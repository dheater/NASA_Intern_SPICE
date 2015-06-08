#Ben Kingen
#11/20/2014
#PY7: Calculating Work Done Taking Ranger to the Moon

#Set-up
from __future__ import division
from visual import *
from visual.graph import *

scene = display(title = "PY7: Calculating Work Done Taking Ranger to the Moon", x = 0, y = 1080,
                width = 960, height = 1080, background = color.black)
scene.range = (4e8, 4e8, 4e8)

#Creation Statements
earth = sphere(pos = (0, 0, 0), radius = 6.37e6, color = color.blue)
moon = sphere(pos = (4e8, 0, 0), radius = 1.75e6, color = (0.5, 0.5, 0.5))
ranger = box(pos = (6.37e6+5e4, 0, 0), size = (4e5, 4e5, 4e5), color = color.red)

egraph = gdisplay(x = 0, y = 0, width = 800, height = 800, title = 'Calculating Work Done Taking Ranger to the Moon',
                  xtitle = 'Time(s)', ytitle = 'Energy(J)', foreground = color.black, background = color.white)
efunction = gcurve(gdisplay = egraph, color = color.red)
kfunction = gcurve(gdisplay = egraph, color = color.green)
ufunction = gcurve(gdisplay = egraph, color = color.blue)

#Initial Conditions and Constants
earth.M = 6e24 #unit: kg
moon.M = 7e22 #unit: kg
ranger.M = 143 #unit: kg
G = 6.67e-11 #unit: N*m**2/kg**2
DT = 5 #unit: s

ranger.p = vector(1.3e4, 0, 0)*ranger.M #unit: kg*m/s
ranger.kf = 0.5*ranger.M*(mag(ranger.p/ranger.M))**2 #unit: J
ranger.ki = ranger.kf #unit: J

w_total = 0

time = 0 #unit: s

#Update Statements
while ranger.pos.x < (moon.pos.x-moon.radius):
    print("velocity:", '%.4f' %mag(ranger.p/ranger.M), "m/s")
    
    erhat = (ranger.pos-earth.pos)/mag(ranger.pos-earth.pos)
    mrhat = (ranger.pos-moon.pos)/mag(ranger.pos-moon.pos)
    
    fer = ((-G*earth.M*ranger.M) / (mag(ranger.pos-earth.pos)**2)) * erhat #unit: N
    fmr = ((-G*moon.M*ranger.M) / (mag(ranger.pos-moon.pos)**2)) * mrhat #unit: N

    uer = ((-G*earth.M*ranger.M) / (mag(ranger.pos-earth.pos))) #unit: N
    umr = ((-G*moon.M*ranger.M) / (mag(ranger.pos-moon.pos))) #unit: N
    
    fnet = fer + fmr #unit: N
    unet = uer + umr
    
    ufunction.plot(pos = (time, unet))
    
    ranger.p += fnet*DT
    dr = ranger.p/ranger.M * DT
    ranger.pos += dr

    w = dot(fnet, dr)
    w_total += w
    
    kfunction.plot(pos = (time, ranger.kf))
    ranger.kf = (mag(ranger.p))**2/(2*ranger.M) #unit: J

    efunction.plot(pos = (time, ranger.kf+unet))
    
    time += DT

print("\nImpact velocity:", '%.4f' %mag(ranger.p/ranger.M), "m/s")
print("Time of impact:", '%.4f' %(time/84600), "days")
print("Initial kinetic:", '%.5g' %ranger.ki, "J")
print("Final kinetic:", '%.5g' %ranger.kf, "J")
print("Change in kinetic:", '%.5g' %(ranger.kf-ranger.ki), "J")
print("Work:", '%.5g' %w_total, "J")

