package tbtGui;
import java.io.*;
import java.awt.*;
import java.awt.image.*;

import javax.swing.*;
import javax.imageio.*;

/**
 * 
 */

/**
 * @author Julien Merienne
 *
 */
public class GameDashBoard extends JPanel {
	// this class is not supposed to be serialized, this field is declared only to get rid of
	// javac warnings
	private static final long serialVersionUID = 0;

	private BufferedImage m_backgroundImage;
	private boolean debug_mode = false;

	/* (non-Javadoc)
	 * @see javax.swing.JComponent#paintComponent(java.awt.Graphics)
	 */
	protected void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		if (debug_mode) return;
		Dimension size = getSize();
		g.drawImage(m_backgroundImage, 0, 0, size.width, size.height, 0, 0,
				m_backgroundImage.getWidth(null), m_backgroundImage
				.getHeight(null), null);
	}
/*
	private class DEBUGJLABEL extends JLabel
	{
		// this class is not supposed to be serialized, this field is declared only to get rid of
		// javac warnings
		private static final long serialVersionUID = 0;
	
	}
*/
	/**
	 * Generate a Left Panel dashbord for the Game screen
	 * @param backgroundImage : the background image to display
	 */
	public GameDashBoard(File backgroundImage) {
		try {
			m_backgroundImage = ImageIO.read(backgroundImage);
		} catch (IOException e) {
			e.printStackTrace();
		}
		//setOpaque(true); 
	}
// DEBUG
	public void debugLog(String msg)
	{
		debug_mode = true;
		removeAll();
		JTextArea debug_text = new JTextArea(47,30);
		String formatted = "";
		int cpt = 0;
		for (int i=0; i < msg.length();i++)
		{
			formatted += msg.charAt(i);
			cpt++;
			if (msg.charAt(i) == '\n') cpt = 1;
			if (cpt%50 == 0) formatted += "\n";

		}
		debug_text.setText(formatted);
		add(debug_text);
		validate();
		repaint();
	}
/*
	public void debugLog(String msg)
	{
		int w = m_backgroundImage.getWidth();
		int h = m_backgroundImage.getHeight();
		BufferedImage dest = new BufferedImage(w, h, m_backgroundImage.getType());
		Graphics2D g2 = dest.createGraphics();
        // Copy src image into dest.
		g2.drawImage(m_backgroundImage, 0, 0, null);
		g2.setPaint(Color.white);
		g2.fillRect(0,0,w,h);
        // Draw your text on top.
        Font font = g2.getFont().deriveFont(12f).deriveFont(Font.BOLD);
        g2.setFont(font);
        FontRenderContext frc = g2.getFontRenderContext();
        float width = (float)font.getStringBounds(msg, frc).getWidth();
        LineMetrics lm = font.getLineMetrics(msg, frc);
        float height = lm.getAscent() + lm.getDescent();
        // Locate text, this will draw it centered
        float fx = (w - width)/2;
        float fy = (h + height)/2 - lm.getDescent();
        g2.setPaint(Color.black);
        g2.drawString(msg, fx, fy);
		g2.dispose();

		//dbImg.setImage(image);
		
		m_backgroundImage = dest;
		
		repaint();
	}*/
}
