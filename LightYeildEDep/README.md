Light Yield and Energy Deposition
=================================

Simulates the effect of the film thickness (energy deposition) on the light yield.

There are two macros provided, one for neutrons and the other for gammas.  Both can be run with the script `subJobs.sh`.

In general it is observed that the light yield increases proportionally to the energy deposition. The script `analysis.py` grabs the mean of both the energy deposition and light yield histograms for each film thickness, while the root script `ExportHistos.C` exports selected histograms of interest to a CSV. 
