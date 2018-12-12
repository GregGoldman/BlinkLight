# BlinkLight
A small project for BME 590: Medical Instrumentation Design
Greg Goldman, ghg3@duke.edu

Project details are copied from https://github.com/mlp6/Fundamentals-BME-Design/blob/master/PrototypingSkills/VariableLightF18.md
There are a few weird commits around the end of October as I was using a loaner computer until I got mine repaired.


Final Specs:
-------------------------------------------------------------
Outer Dimensions: 1.510 x 1.870 x 0.850 inches


Weight: 1.15 ounces


Cost: $5.81 (including batteries, not including board and 3D-printed case)

Battery Life (when using two CR2430 batteries in series):

Asleep: 9232.5 hours = 384.7 days

Low:    67.2   hours = 2.8   days

Medium 	43.2   hours = 1.8   days

High	25.2   hours = 1.05  days

Flash	37.6   hours = 1.57  days (assuming average of 4 and 13 mA)

The design specs say use 1 battery up to 9 V. If you consider each CR2430
to be a one cell in a two-cell battery which is composed of two CR2430s taped
together, then it's only one battery ;)


Improvements:
-------------------------------------------------------------
Use a LDO which regulates to 2.7 V (ATTiny 85 minimum operating voltage)
so that I could use a single 3V CR2430 or similar battery and waste less power 
through the regulator. I had chosen a 3.3 V regulator because the LED had a 
voltage drop of 3.2 V, but I could likely find a similar one with a smaller voltage drop.

The layout could be improved. The placement of the LED was originally based on trying to model
the case like the Millenium Falcon, but it looked too thick to be satisfactory. As such, the 
LED leads and corresponding traces are longer than they need to be. 

Using a battery with a smaller diameter would have allowed for a smaller overall design.
The primary restriction was the size of the battery holder.


Final important documents:
-------------------------------------------------------------
Schematic and E-CAD photos: \BlinkLight\Electrical\FINAL OUTPUTS
Mechanical photos and drawings: \BlinkLight\Mechanical\Final Design

# Design Skills Project (Variable Light Intensity)

## Functional Specifications
* LED light that is button-activated to switch between the following operating modes:
  1. `Off`
  1. `Brightest`
  1. `Intermediate Brightness`
  1. `Least Bright`
  1. `Flashing @ 2 Hz`
* Self-contained device with LED and button exposed.
* Extra credit possibilities:
  + Battery life indicator
  + Waterproof enclosure
  + You tell us!

## Constraints
* Run on single battery up to 9 V
* Electronics connected to a single PCB (can be single- or double-sided)
* Cannot weight more than 1 lb
* Cannot have a single dimension > 6 in
* Cannot cost more than $50 to duplicate unit
* Battery must be easily replaceable

## Evaluation
* Minimize weight
* Minimize size
* Maximize `Brightest` mode
* Minimize power draw / maximize battery life
* Survive 3 foot drop with battery installed

* Create-and-maintain a GitHub project repository to maintain version history of:
  + Mechanical CAD (PDFs + more)
  + Electronic CAD (PDFs + more)
  + Software
  + Testing data
  + Design history / notes
  + Specification sheets
  + Usage notes (`README.md`)
  + Kanban board / Issues / Milestones
