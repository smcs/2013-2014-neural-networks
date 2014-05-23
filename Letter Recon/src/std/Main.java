package std;

import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import ij.*;
import ij.io.*;
import ij.process.BinaryProcessor;
import ij.process.ByteProcessor;
import ij.process.ImageProcessor;

public class Main {
	public static double xCentroid;
	public static double yCentroid;
	public static double aspectRatio;
	public static int grid[][] = new int[20][20];
	public static void main(String[] args) {
		/*
		 * An opener is just a utility object to open images -- it could easily
		 * be connected to an open dialog instead of a hard-coded file path
		 */
		for (int i=0;i<20;i++){
			for (int j=0;j<20;j++){
				grid[i][j]=0;
			}
		}
		Opener opener = new Opener();

		/* An ImagePlus is an object that represents the loaded image */
		//ImagePlus a = opener.openImage("images/A.png");
		/* An ImagePlus is an object that represents the loaded image. IJ is the ImageJ application. */
		ImagePlus image = IJ.openImage("images/U.png");
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
			cropAndResizeSkeletonize(image, 200, 200);
		} catch (IOException e) {
			System.err.println("IOException: " + e.getMessage());
		}
		
		xCentroid=centroid(grid)[0];
		yCentroid=centroid(grid)[1];
		System.out.println("xCentroid:"+xCentroid+" yCentroid:"+yCentroid);
		System.out.println(nPixel(image.getProcessor()));
	}
		
		public static void cropAndResizeSkeletonize(ImagePlus imp, int targetWidth, int targetHeight) throws IOException {
		    ImageProcessor ip = imp.getProcessor();
		    //System.out.println("size1: "+ip.getWidth()+"x"+ip.getHeight());
		    ip.setInterpolationMethod(ImageProcessor.BILINEAR);
		    //System.out.println("size2: "+ip.getWidth()+"x"+ip.getHeight());
		 
		    int[] whereToCrop = findBoundary(ip);
		    
		    
		    int cropX = whereToCrop[2];
		    int cropY = whereToCrop[0];
		    int tWidth = whereToCrop[4];
		    int tHeight = whereToCrop[5];
		    
		    ip.setRoi(cropX, cropY, tWidth, tHeight);
		    System.out.println(cropX + " " + cropY + " " + tWidth + " " + tHeight);
		    ip = ip.crop();
		    aspectRatio=(double)ip.getBufferedImage().getWidth()/(double)ip.getBufferedImage().getHeight();
		    System.out.println("Aspect Ratio: " + aspectRatio);
		    //System.out.println("size3: "+ip.getWidth()+"x"+ip.getHeight());
		    ip = ip.resize(100, 100);
		    BufferedImage croppedImage = ip.getBufferedImage();
		 
		    //System.out.println("size4: "+ip.getWidth()+"x"+ip.getHeight());
		    //new ImagePlus("croppedImage", croppedImage).show();
		 
		    ImageIO.write(croppedImage, "jpg", new File("cropped.jpg"));
		    BinaryProcessor bip = new BinaryProcessor(new ByteProcessor(croppedImage));
		    bip.skeletonize();
		    new ImagePlus("SkeletonizedImage", bip.getBufferedImage()).show();
		    toGrid(bip);
		    for (int i=0;i<20;i++){
				for (int j=0;j<20;j++){
					System.out.print(grid[i][j] + " ");
				}
				System.out.println();
			}
		    System.out.println("MeanSquareHorizontal: " +MeanSquareHorizontal(bip));
		    System.out.println("MeanSquareVertical: " + MeanSquareVertical(bip));
		  }
	
		public static double nPixel(ImageProcessor imp) {
			imp.resize(100,100);
			double count=0.0;
			for (int i=0;i<imp.getWidth();i++){
				for (int j=0;j<imp.getHeight();j++){
					if (imp.get(i,j) != 255){
						count=count+1.0;
					}
				}
			}
			return count/10000;
		}
		
		public static int[] findBoundary(ImageProcessor imp) { 
            Rectangle r = imp.getRoi(); 
                    
            int minX = 0; 
            int minY = 0; 
            int maxX = 0; 
            int maxY = 0; 
            
            for (int y=0; y<imp.getHeight(); y++) { 
                    for (int x=0; x<imp.getWidth(); x++) { 
                            if(imp.get(x, y) != 255) { 
                                    maxY = y; 
                                break; 
                            } 
                    } 
            } 
            
            for (int x=0; x<imp.getWidth(); x++) { 
                    for (int y=0; y<imp.getHeight(); y++) { 
                            if(imp.get(x, y) != 255) { 
                                    maxX = x; 
                                break; 
                            } 
                    } 
            } 
            
            for (int y=imp.getHeight()-1; y>=0; y--) { 
                    for (int x=0; x<imp.getWidth(); x++) { 
                            if(imp.get(x, y) != 255) { 
                                    minY = y; 
                                break; 
                            } 
                    } 
            } 
            
            
            for (int x=imp.getWidth()-1; x>=0; x--) { 
                    for (int y=0; y<imp.getHeight(); y++) { 
                            if(imp.get(x, y) != 255) { 
                                    minX = x; 
                                break; 
                            } 
                    } 
            } 
            
            int newWidth = maxX-minX; 
            int newHeight = maxY-minY; 
            
            int[] boundary =  {minY, maxY, minX, maxX, newWidth, newHeight};
            
            return boundary;           
        } 
		public static void toGrid(BinaryProcessor imp) { 
			double unitx=imp.getWidth()/20.0;
			double unity=imp.getHeight()/20.0;
			System.out.println(unitx + " " + unity);
			for (int i=0;i<imp.getWidth();i++){
				for (int j=0;j<imp.getHeight();j++){
					if (imp.getPixel(i, j)!=255){
						grid[(int)(i/unitx)][(int)(j/unity)]=1;
					}
				}
			}
		}
		
		public static double[] centroid(int[][] grid){
			double xSum=0.0,ySum=0.0;
			int count=0;
			for (int i=0;i<20;i++){
				for (int j=0;j<20;j++){
					if (grid[i][j]==1){
						count=count+1;
					}
					xSum+=grid[i][j]*i;
					ySum+=grid[i][j]*j;
				}
			}
			xSum=xSum/count;
			ySum=ySum/count;
			double[] CentroidValue={xSum,ySum};
			return CentroidValue;
		}
		
		public static double MeanSquareHorizontal(BinaryProcessor imp){
			double HorizontalMeanSquare=0.0;
			int[] temp=new int[imp.getWidth()];
			int halfHeight=imp.getHeight()/2;
			int p1=0,p2=0,ep=imp.getWidth()-1;;
			int count=0;
			int flag=0;
			for (int i=0;i<imp.getWidth();i++){
				temp[i]=0;
				temp[i]=imp.get(i,halfHeight);
			}
			while (temp[p1]==255){
				p1=p1+1;
			}
		    while (temp[ep]==255){
		    	ep=ep-1;
		    }
			while ((p1<ep)&&(p2<ep)){
				if (p1<imp.getWidth()){
				    while ((temp[p1]!=255)&&(p1<ep)){
					    p1=p1+1;
					    if (p1>=ep){
					    	break;
					    }
				    }
				}
				p2=p1+1;
				if (p2<ep){
				    while (temp[p2]==255){
					    p2=p2+1;
					    if (p2>=ep){
					    	break;
					    }
				    }
		        }
				if (p2-p1>5){
					count=p2-p1;
					HorizontalMeanSquare+=count*count;
					flag=flag+1;
				}
				p1=p2;
				p2=p1+1;
			}
			if (flag==0){
				flag=1;
			}
			return HorizontalMeanSquare/flag;
		}
		public static double MeanSquareVertical(BinaryProcessor imp){
			double VerticalMeanSquare=0.0;
			int[] temp=new int[imp.getHeight()];
			int halfWidth=imp.getWidth()/2;
			int p1=0,p2=0,ep=imp.getHeight()-1;;
			int count=0;
			int flag=0;
			for (int i=0;i<imp.getHeight();i++){
				temp[i]=0;
				temp[i]=imp.get(halfWidth,i);
			}
			while (temp[p1]==255){
				p1=p1+1;
			}
		    while (temp[ep]==255){
		    	ep=ep-1;
		    }
			while ((p1<ep)&&(p2<ep)){
				if (p1<imp.getHeight()){
				    while ((temp[p1]!=255)&&(p1<ep)){
					    p1=p1+1;
					    if (p1>=ep){
					    	break;
					    }
				    }
				}
				p2=p1+1;
				if (p2<ep){
				    while (temp[p2]==255){
					    p2=p2+1;
					    if (p2>=ep){
					    	break;
					    }
				    }
		        }
				if (p2-p1>5){
					count=p2-p1;
					VerticalMeanSquare+=count*count;
					flag=flag+1;
				}
				p1=p2;
				p2=p1+1;
			}
			if (flag==0){
				flag=1;
			}
			return VerticalMeanSquare/flag;
		}
		
		
		/* 1. Aspect Ratio: Width/Height (Normalized by 3 to account for letters such as M) 
		 * 2. CentroidX
		 * 3. CentroidY
		 * 4. Number of Pixel (Normalized by total after resizing the image)
		 * 5. Mean Squared Value (Lengths of segments of horizontal cut [>6 pixels, halfway height])
		 * 6. Mean Squared Value (Lengths of segments of vertical cut [>6 pixels, halfway width])
		 * 7.
		 */
}