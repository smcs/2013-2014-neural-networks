package std;

import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import ij.*;
import ij.io.*;
import ij.process.ImageProcessor;

public class Main {

	public static void main(String[] args) {
		/*
		 * An opener is just a utility object to open images -- it could easily
		 * be connected to an open dialog instead of a hard-coded file path
		 */
		Opener opener = new Opener();

		/* An ImagePlus is an object that represents the loaded image */
		//ImagePlus a = opener.openImage("images/A.png");
		/* An ImagePlus is an object that represents the loaded image. IJ is the ImageJ application. */
		ImagePlus image = IJ.openImage("images/A.png");
		/* display the image we're working with in its own window -- this also puts ImageJ's focus on this image, so that subsequent commands will run on this image. */
		image.show();
		
	/*
		 * the pixel data represents the pixel as RGBA (Red, Green, Blue and
		 * Alpha) channels, where each channel has a value 0-255, and alpha
		 * represents transparency (0 is opaque, 255 is completely transparent)
		 */
		//int[] pixelData = a.getPixel(36, 25);
		
		int[] pixelData = image.getPixel(36, 25);
    	System.out.println(pixelData[0] + " " + pixelData[1] + " "
				+ pixelData[2] + " " + pixelData[3]);
		
		/* convert the current image to 8-bit -- calling the command by its exact name in the menu */
		IJ.run("8-bit");
		
		try{
		cropAndResize(image, 400, 100);
		} catch (IOException e) {
			System.err.println("IOException: " + e.getMessage());
		}
	}
		
		public static void cropAndResize(ImagePlus imp, int targetWidth, int targetHeight) throws IOException {
		    ImageProcessor ip = imp.getProcessor();
		    System.out.println("size1: "+ip.getWidth()+"x"+ip.getHeight());
		    ip.setInterpolationMethod(ImageProcessor.BILINEAR);
		    ip = ip.resize(targetWidth * 2, targetHeight * 2);
		    System.out.println("size2: "+ip.getWidth()+"x"+ip.getHeight());
		 
		    int[] whereToCrop = findBoundary(ip);
		    
		    
		    int cropX = whereToCrop[2];
		    int cropY = whereToCrop[0];
		    targetWidth = whereToCrop[4];
		    targetHeight = whereToCrop[5];
		    
		    ip.setRoi(cropX, cropY, targetWidth, targetHeight);
		    ip = ip.crop();
		    System.out.println("size3: "+ip.getWidth()+"x"+ip.getHeight());
		    BufferedImage croppedImage = ip.getBufferedImage();
		 
		    System.out.println("size4: "+ip.getWidth()+"x"+ip.getHeight());
		    new ImagePlus("croppedImage", croppedImage).show();
		 
		    ImageIO.write(croppedImage, "jpg", new File("cropped.jpg"));
		  }
	
		
		
		public static int[] findBoundary(ImageProcessor imp) { 
            Rectangle r = imp.getRoi(); 
                    
            int minX = 0; 
            int minY = 0; 
            int maxX = 0; 
            int maxY = 0; 
            
            for (int y=0; y<imp.getHeight(); y++) { 
                    for (int x=0; x<imp.getWidth(); x++) { 
                            if(imp.get(x, y) == 255) { 
                                    maxY = y; 
                                break; 
                            } 
                    } 
            } 
            
            for (int x=0; x<imp.getWidth(); x++) { 
                    for (int y=0; y<imp.getHeight(); y++) { 
                            if(imp.get(x, y) == 255) { 
                                    maxX = x; 
                                break; 
                            } 
                    } 
            } 
            
            for (int y=imp.getHeight()-1; y>=0; y--) { 
                    for (int x=0; x<imp.getWidth(); x++) { 
                            if(imp.get(x, y) == 255) { 
                                    minY = y; 
                                break; 
                            } 
                    } 
            } 
            
            
            for (int x=imp.getWidth()-1; x>=0; x--) { 
                    for (int y=0; y<imp.getHeight(); y++) { 
                            if(imp.get(x, y) == 255) { 
                                    minX = x; 
                                break; 
                            } 
                    } 
            } 
            
            int newWidth = (imp.getWidth()-(imp.getWidth()-maxX-1)); 
            int newHeight = (imp.getHeight()-(imp.getHeight()-maxY-1)); 
            
            int[] boundary =  {minY, maxY, minX, maxX, newWidth, newHeight};
            
            return boundary;
            
            //IJ.run(imp, "Canvas Size...", "width="+newWidth+" height="+newHeight+" position=Top-Left zero"); 
            //IJ.run(imp, "Canvas Size...", "width="+(newWidth-minX)+" height="+(newHeight-minY)+" position=Bottom-Right zero"); 
            
            /*IJ.write("New Width: "+newWidth); 
            IJ.write("New Height: "+newHeight); 
            
            IJ.write("found min Y at: " + minY + "\n"); 
            IJ.write("found max Y at: " + maxY + "\n"); 
            IJ.write("found min X at: " + minX + "\n"); 
            IJ.write("found max X at: " + maxX + "\n");*/ 
} 
}