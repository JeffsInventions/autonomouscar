import cv

# setup webcam
capture = cv.CaptureFromCAM(0)

# set resolution
cv.SetCaptureProperty(capture,cv.CV_CAP_PROP_FRAME_WIDTH,320)
cv.SetCaptureProperty(capture,cv.CV_CAP_PROP_FRAME_HEIGHT,240)

# setup file output
filenm = "output.avi"
codec = 0
fps = 15
width = int(cv.GetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_WIDTH))
height = int(cv.GetCaptureProperty(capture, cv.CV_CAP_PROP_FRAME_HEIGHT))
writer = cv.CreateVideoWriter(filenm, codec, fps, (width,height), 1)



# display frame on screen and write it to disk
def DisplayWriteFrame(image):
	cv.ShowImage('Image_Window',image)
	
	i = 0
	while i < 15:
		cv.WriteFrame(writer, image)
		i+=1

	cv.WaitKey(1000)


j = 0
while j < 5:
	# capture frame from webcam
	rawimage = cv.QueryFrame(capture)
	
	DisplayWriteFrame(rawimage)
		
	# convert frame to greyscale
	depth = 8
	channels = 1
	greyscaleimage = cv.CreateImage((width,height),depth,channels)
	cv.CvtColor(rawimage,greyscaleimage,cv.CV_BGR2GRAY)
	
	DisplayWriteFrame(greyscaleimage)
	
	# perform canny edge detection on frame
	depth = 8
	channels = 1
	lowerthreshold = 10
	upperthreshold = 100
	aperture = 3
	edgeimage = cv.CreateImage((width,height),depth,channels)
	cv.Canny(greyscaleimage,edgeimage,lowerthreshold,upperthreshold,aperture)
	
	DisplayWriteFrame(edgeimage)
	
	j+=1