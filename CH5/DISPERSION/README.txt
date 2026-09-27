Chapter 4 problem 7:

Data on the dispersion of light in water is given in Table 4.3. Interpolate a smooth function for these data. Plot the function together with the input data.

Notes:
	(I figured out I can put symbols like √∫∂π₀ using Mac.)	

	const Tuple->read(saveTo, col)
	Method used for reading Table data.

	CubicSplinePolynomial must be promoted to a PlotFunction1D so that we may see it.


Solution:
	The Cubic Spline method is extraordinarily accurate as it does not just rely on straight lines between datapoints. It fits one beast of a polynomial to the data points. This problem had me searching through the manual. After a while of searching through the manual, I had AI index it for parts I was looking for. I turned off training for the AI model. 

Issue:
	I was not able to figure out how to splice the view into multiple views to present both the table and the plot.



	