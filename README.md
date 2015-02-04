# fermentmonitor
QT project for a Beagle to help me monitor fermentation temps and progress.
This is a custom app that is designed to sit next to one or more fementation
vessels and use a BeagleBone or RPI to display that information locally, and
a REST service to allow for remote queries by another web page, or an app.

It makes use of cheap 1-wire thermometers and some IR photo interrupters to
measure fermentation temps and to count bubbles as it goes, allowing the
service to flag when fermentation is done, now effectively just flaggin
12 hours without a new bubble counted.

More to come.
