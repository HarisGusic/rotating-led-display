
import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.PrintWriter;
import java.util.Scanner;

import javax.imageio.ImageIO;

public class Main {

	public static void main(String args[]) throws Exception {

		Scanner scanner = new Scanner(System.in);
		String filename = scanner.nextLine();
		BufferedImage image = null;
		image = ImageIO.read(
				new File(Main.class.getProtectionDomain().getCodeSource().getLocation().toURI().getPath() + filename));
		PrintWriter pw = null;
		filename = filename.substring(0, filename.length() - 4) + ".txt";
		pw = new PrintWriter(filename);
		String output = "{\n";

		for (double i = 0; i < image.getHeight(); i++) {
			if (i > 0)
				output += ",\n";
			output += "{";
			for (double j = 0; j < image.getWidth(); j++) {
				if (j > 0)
					output += ", ";
				int rgb = image.getRGB((int) j, (int) i) & 0xffffff;
				int r = rgb >> 16 & 0xff, g = rgb >> 8 & 0xff, b = rgb & 0xff;
				output += "0x" + (r > 0 ? Integer.toHexString(r) : "00") + (g > 0 ? Integer.toHexString(g) : "00")
						+ (b > 0 ? Integer.toHexString(b) : "00");
			}
			output += " }";
		}
		output += "\n};";
		pw.write(output);
		pw.close();
		Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(output), null);
		System.out.println("Gotovo!");
		scanner.close();
	}

}