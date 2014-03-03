import java.util.*;
import java.io.*;

public class BPNet {

	public static int InNode = 4;
	public int HiddenNode = 10;
	public static int OutNode = 1;
	public static int SampleSize = 256;

	Random random = new Random();

	public static double[][] Data = new double[SampleSize][InNode];
	public static double[][] DataTarget = new double[SampleSize][OutNode];

	public double[] HiddenThresh = new double[HiddenNode];
	public double[] OutThresh = new double[OutNode];
	public double[][] HiddenWeight = new double[InNode][HiddenNode];
	public double[][] OutWeight = new double[HiddenNode][OutNode];
	public double rate_HiddenThresh=0;
	public double rate_HiddenWeight=0;
	public double rate_OutThresh=0;
	public double rate_OutWeight=0;
	public double error=0;
	public double MaxError=0;
	public double[] result = new double[OutNode];

	public BPNet() {
		error = 0;
		MaxError = 1;
		rate_HiddenThresh = 0.9;
		rate_HiddenWeight = 0.9;
		rate_OutThresh = 0.9;
		rate_OutWeight = 0.9;
	}

	public void init() {
		for (int i = 0; i < InNode; i++) {
			for (int j = 0; j < HiddenNode; j++) {
				HiddenWeight[i][j] = (random.nextDouble() * 2) - 1; // -1 to 1
			}
		}
		for (int i = 0; i < HiddenNode; i++) {
			for (int j = 0; j < OutNode; j++) {
				OutWeight[i][j] = (random.nextDouble() * 2) - 1;
			}
			HiddenThresh[i] = (random.nextDouble() * 2) - 1;
		}
		for (int i = 0; i < OutNode; i++) {
			OutThresh[i] = (random.nextDouble() * 2) - 1;
		}
	}

	public void train() {
		double[] Input = new double[InNode];
		double[] Hidden = new double[HiddenNode];
		double[] Output = new double[OutNode];
		double[] Target = new double[OutNode];
		double[] TargetDif = new double[OutNode];
		double[] HiddenAdjust = new double[HiddenNode];
		double[] HiddenAct = new double[HiddenNode];
		double[] OutAct = new double[OutNode];

		for (int Sample = 0; Sample < SampleSize; Sample++) {
			for (int i = 0; i < InNode; i++) {
				Input[i] = 0;
				Input[i] = Data[Sample][i];
			}
			for (int i = 0; i < OutNode; i++) {
				Target[i] = 0;
				Target[i] = DataTarget[Sample][i];
			}
			for (int i = 0; i < HiddenNode; i++) {
				HiddenAct[i] = 0.0;
				for (int j = 0; j < InNode; j++) {
					HiddenAct[i] = HiddenAct[i] + HiddenWeight[j][i] * Input[j]; // Get
																					// the
																					// activation
																					// value
																					// for
																					// the
																					// hidden
																					// nodes
				}
				Hidden[i] = (double) 1.0
						/ (1.0 + Math.pow(Math.E, -HiddenThresh[i]
								- HiddenAct[i]));// Sigmoid function for Hidden
													// output
			}
			for (int i = 0; i < OutNode; i++) {
				OutAct[i] = 0.0;
				for (int j = 0; j < HiddenNode; j++) {
					OutAct[i] = OutAct[i] + OutWeight[j][i] * Hidden[j]; // Get
																			// the
																			// activation
																			// value
																			// for
																			// the
																			// output
																			// nodes
				}
				Output[i] = (double) 1.0
						/ (1.0 + Math.pow(Math.E, -OutThresh[i] - OutAct[i])); // Sigmoid
																				// function
																				// for
																				// Output
			}
			/*
			 * Adjust the OutWeight
			 */
			for (int i = 0; i < OutNode; i++) {
				TargetDif[i] = (1 - Output[i]) * Output[i]
						* (Target[i] - Output[i]); // Find the error of single
													// outnode
				for (int j = 0; j < HiddenNode; j++) {
					OutWeight[j][i] = OutWeight[j][i] + rate_OutWeight
							* TargetDif[i] * Hidden[i];
				}
				// alpha * s'(a(p,n)) * (t(p,n) - y(p,n)) * X(p,i,n).
			}
			/*
			 * Adjust the HiddenWeight
			 */
			for (int i = 0; i < HiddenNode; i++) {
				HiddenAdjust[i] = 0.0;
				for (int j = 0; j < OutNode; j++) {
					HiddenAdjust[i] = HiddenAdjust[i] + TargetDif[j]
							* OutWeight[i][j];
				}
				HiddenAdjust[i] = (1 - Hidden[i]) * Hidden[i] * HiddenAdjust[i];
				for (int j = 0; j < InNode; j++) {
					HiddenWeight[j][i] = HiddenWeight[j][i] + rate_HiddenWeight
							* HiddenAdjust[i] * Input[j];
				}
				// alpha * s'(a(p,n)) * sum(d(j) * W(n,j)) * X(p,i,n)
			}
			for (int i = 0; i < OutNode; i++) {
				error = error + Math.abs(Target[i] - Output[i])
						* Math.abs(Target[i] - Output[i]);
			}
			/*
			 * Adjust the Threshholds of hiddennodes and outnodes
			 */
			MaxError = error / 2.0;
			for (int i = 0; i < HiddenNode; i++) {
				HiddenThresh[i] = HiddenThresh[i] + rate_HiddenThresh
						* HiddenAdjust[i];
			}
			for (int i = 0; i < OutNode; i++) {
				OutThresh[i] = OutThresh[i] + rate_OutThresh * TargetDif[i];
				// cout<<OutThresh[i];
			}
		}
	}

	public void recognize(double[] data) {
		double[] Input = new double[InNode];
		double[] Hidden = new double[HiddenNode];
		double[] Output = new double[OutNode];

		double[] actHidden = new double[HiddenNode];
		double[] actOut = new double[OutNode];

		for (int i = 0; i < InNode; i++) {
			Input[i] = data[i];
		}
		for (int i = 0; i < HiddenNode; i++) {
			actHidden[i] = 0.0;
			for (int j = 0; j < InNode; j++) {
				actHidden[i] = actHidden[i] + HiddenWeight[j][i] * Input[j];
			}
			Hidden[i] = (double) 1.0
					/ (1.0 + Math.pow(Math.E, -HiddenThresh[i] - actHidden[i]));
		}
		for (int i = 0; i < OutNode; i++) {
			actOut[i] = 0.0;
			for (int j = 0; j < HiddenNode; j++) {
				actOut[i] = actOut[i] + OutWeight[j][i] * Hidden[j];
			}
			Output[i] = (double) 1.0
					/ (1.0 + Math.pow(Math.E, -OutThresh[i] - actOut[i]));
		}
		for (int i = 0; i < OutNode; i++) {
			result[i] = Output[i];
		}

	}

	public static void main(String args[]) {
		int[] ActualAnswer = new int[256];

		// ofstream myFile("OutputWeights.txt");
		BPNet BP = new BPNet();
		BP.init();
		int count = 0;

		double[] temp= {0.0,0.0,0.0,0.0};
		
		Scanner scanner = new Scanner (System.in);
		//Console console = System.console();

		for (int i=0;i<InNode;i++){
		 //String[] tempStr = null;
		 //tempStr[i] = console.readLine("Enter input:");
		
		 System.out.println("Enter "+(i+1)+" entry");  
		 temp[i] = scanner.nextDouble();
		 
		 //temp[i] = Double.parseDouble(tempStr[i]);
		 }
		for (int a = 0; a < 4; a++) {
			for (int b = 0; b < 4; b++) {
				for (int c = 0; c < 4; c++) {
					for (int d = 0; d < 4; d++) {
						Data[count][0] = a;
						Data[count][1] = b;
						Data[count][2] = c;
						Data[count][3] = d;
						ActualAnswer[count] = a * d - b * c;
						count++;
					}
				}
			}
		}
		for (int i = 0; i < SampleSize; i++) {
			DataTarget[i][0] = (double) i * (1.0 / ((double) SampleSize - 1.0)); // ??
		}
		// system("PAUSE");

		count = 0;
		while (BP.MaxError > 0.00085) {
			count++;
			BP.error = 0.0;
			BP.train();
			// cout<<count<<" "<<"error="<<BP.MaxError<<endl;
			System.out.println("error " + BP.error);
		}
		BP.recognize(temp);
		double min = 10000;
		double[][] diff = new double[SampleSize][OutNode];
		int index = 0;
		// cout<<BP.result[0]<<endl;
		System.out.println("determinant " + BP.result[0]);

		for (int i = 0; i < SampleSize; i++) {
			for (int j = 0; j < OutNode; j++) {
				diff[i][j] = (double) Math.abs(DataTarget[i][j] - BP.result[j]);
				// cout<<diff[i][j]<<endl;
				System.out.println("difference[" + i + "," + j + "]: "
						+ diff[i][j]);

				if (min > diff[i][j]) {
					min = diff[i][j];
					index = i;
				}
			}
		}

		/*
		 * if (myFile.is_open()){ myFile<<"Weights from input to hidden:"<<endl;
		 * for (int i=0;i<InNode;i++){ for (int j=0;j<HiddenNode;j++){
		 * myFile<<BP.HiddenWeight[i][j]<<" "; } myFile<<endl; } myFile<<endl;
		 * myFile<<"Wights from hidden to output:"<<endl; for (int
		 * i=0;i<HiddenNode;i++){ for (int j=0;j<OutNode;j++){
		 * myFile<<BP.OutWeight[i][j]<<" "; } myFile<<endl; }
		 * myFile<<endl<<"Input-output matching:"<<endl; myFile<<"Input: "; for
		 * (int i=0;i<SampleSize;i++){ myFile<<"{"; for (int j=0;j<InNode;j++){
		 * myFile<<Data[i][j]; if (j!=InNode-1) myFile<<","; } myFile<<"} "; }
		 * myFile<<endl<<"Target: "; for (int i=0;i<SampleSize;i++){
		 * myFile<<DataTarget[i][0]<<" | "; } myFile<<endl; }
		 */
		System.out.println("index"+ActualAnswer[index]); 
	}

}
