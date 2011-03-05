package tbtGui;
/**
 * 
 */

import java.io.*;
import java.awt.*;
import java.awt.image.*;

import javax.swing.*;
import javax.imageio.*;

/**
 * @author Julien Merienne
 * Menu screen
 */
public class Menu extends JPanel {
	// this class is not supposed to be serialized, this field is declared only to get rid of
	// javac warnings
	private static final long serialVersionUID = 0;

	private BufferedImage m_backgroundImage;

	/* (non-Javadoc)
	 * @see javax.swing.JComponent#paintComponent(java.awt.Graphics)
	 */
	protected void paintComponent(Graphics g)
	{
		super.paintComponent(g);
	 
		Dimension size = getSize();
		g.drawImage(m_backgroundImage, 0, 0, size.width, size.height, 0, 0,
				m_backgroundImage.getWidth(null), m_backgroundImage
				.getHeight(null), null);
	}

	/**
	 * Generate a Menu Screen
	 * @param maxSize : the maximum size allowed for this container
	 * @param backgroundImageFilename : path to the background image to display in the menu
	 */
	public Menu(String backgroundImageFilename, Dimension maxSize) {
		try {
			m_backgroundImage = ImageIO.read(new File(backgroundImageFilename));
		} catch (IOException e) {
			e.printStackTrace();
		}
		setOpaque(true); 
	}
}