# BlinkLight
A small project for BME 590: Medical Instrumentation Design
Greg Goldman, ghg3@duke.edu

Project details are copied from https://github.com/mlp6/Fundamentals-BME-Design/blob/master/PrototypingSkills/VariableLightF18.md
There are a few weird commits around the end of October as I was using a loaner computer until I get mine repaired.


$Final Specs:
-------------------------------------------------------------
Outer Dimensions: 1.510 x 1.870 x 0.850 inches


Weight:


Cost: $5.81 (including batteries, not including board and 3D-printed case)

Battery Life (when using two CR2430 batteries in series):
Asleep: 9232.5 hours = 384.7 days
Low:    67.2   hours = 2.8   days
Medium 	43.2   hours = 1.8   days
High	25.2   hours = 1.05  days
Flash	37.6   hours = 1.57  days
(assuming average of 4 and 13 mA)




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
