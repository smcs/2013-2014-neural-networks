package std;

import ij.ImagePlus;
import ij.io.*;
import ij.process.*;
import ij.plugin.*;

public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Opener opener = new Opener();
		ImagePlus image = new ImagePlus();
		image = opener.openImage("A.PNG");
		image.show();
	}
}
