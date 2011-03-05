package tbtGui;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
//import javax.swing.border.*;
import javax.swing.*;

/**
 * 
 */

/**
 * @author Julien Merienne
 * The Tbt Gui Main Frame
 */
public class TbtMainFrame extends JFrame {
	// this class is not supposed to be serialized, this field is declared only to get rid of
	// javac warnings
	private static final long serialVersionUID = 0;
	
	public static TbtMainFrame c_TbtJFrame = null;

	// used to manage fullScreen and windowed modes, though for now tbt is only supported in
	// fullScreen mode
	private Rectangle m_framePosition = new Rectangle();
	private boolean m_isFullScreen = false;
	private boolean m_processWindowEvents = true;
	private Cursor m_emptyCursor = createEmptyCursor();
	
	// A background JPanel which cover all the JFrame 
	private JPanel m_background = null;
	// The JPanel in which things will be displayed, is inside m_background
	private DisplayJPanel m_display = null;
	
	/**
	 * The Main Window
	 */
	public TbtMainFrame() {
		setVisible(false);
		setSize(Toolkit.getDefaultToolkit().getScreenSize());
		setLayout(new BorderLayout());
		bindKeys();

		m_background = new JPanel();
	//	m_background.setBorder(new LineBorder(Color.RED, 1));
		m_background.setBackground(Color.BLACK);
		add(m_background, BorderLayout.CENTER);

		m_display = new DisplayJPanel(getSize());
	//	m_display.setBorder(new LineBorder(Color.RED, 1));
		m_background.add(m_display);
		
		updateBackgroundLayout();
		
		display(true,true);
	}

	private void bindKeys()
	{
		// Bind the 'ESCAPE' key event
		KeyStroke kesc = KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0, false);
		getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(kesc, "ESCAPE");
		getRootPane().getActionMap().put("ESCAPE", new QuitGame());
	}

	/**
	 * Since the main JPanel m_display displayed over the m_background JPanel may change its
	 * size, this method allow to update m_background's SpringLayout constraints in order to
	 *  keep the m_display JPanel centered
	 */
	public void updateBackgroundLayout()
	{
		Dimension displaySize = m_display.getSize();

		SpringLayout layout = new SpringLayout();

		layout.putConstraint(SpringLayout.WEST, m_display,
				(int)(getSize().getWidth() - displaySize.getWidth())/2,
                SpringLayout.WEST, m_background);
		layout.putConstraint(SpringLayout.NORTH, m_display,
				(int)(getSize().getHeight() - displaySize.getHeight())/2,
                SpringLayout.NORTH, m_background);

 		m_background.setLayout(layout);
	}

	public void display(boolean show, boolean fullScreen) {
		setVisible(false);
		setFullScreen(fullScreen);
		setVisible(show);
	}
	
	private Cursor createEmptyCursor() {
		int[] pixels = new int[0/* 16 * 16 */];
		Image image = Toolkit.getDefaultToolkit().createImage(
				new MemoryImageSource(0, 0, pixels, 0, 0));
		return Toolkit.getDefaultToolkit().createCustomCursor(image,
				new Point(0, 0), "InvisibleCursor");
	}

	private class QuitGame extends AbstractAction{
		// this class is not supposed to be serialized, this field is declared only to get rid
		// of javac warnings
		private static final long serialVersionUID = 0;
		public void actionPerformed(ActionEvent e){
		     System.exit(0);
		}
	}
	 
	private Timer timeToHideMouse = new Timer(1500, new HideMouse());

	private class HideMouse implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			setCursor(m_emptyCursor);
		}
	}

	private AWTEventListener scheduleHideMouse = new HideMouseStart();

	private class HideMouseStart implements AWTEventListener {
		public void eventDispatched(AWTEvent e) {
			if (m_isFullScreen) {
				setCursor(Cursor.getDefaultCursor());
				timeToHideMouse.restart();
				timeToHideMouse.setRepeats(false);
			}
		}
	}

	public static void reDraw()
	{
		if (c_TbtJFrame != null)
		{
			// not finding the JFrame in c_TbtJFrame mean
			// we are still building it and nothing is shown yet.
			// so there us no need to call updateBackgroundLayout() at
			// this time, or to repaint it, it will be done soon elsewhere
			// (besides even if we wanted ton we couldn't ^^)
			
			// we found the JFrame in c_TbtJFrame, so update layouts and repaint
			TbtMainFrame.c_TbtJFrame.validate();
			TbtMainFrame.c_TbtJFrame.updateBackgroundLayout();
			TbtMainFrame.c_TbtJFrame.repaint();
		}
	}
	
	public boolean isFullScreen() {
		return m_isFullScreen;
	}

	/**
	 * This method places the JFrame in full screen mode. This means 1) Not
	 * resizable. 2) Undecorated if possible. 3) Always on top. Moreover it also
	 * installs a mouse listener that hides the cursor if it is not moved in
	 * more than 1.5s and shows it when moved.
	 * 
	 * @param setFull :
	 *            set the fullscreen
	 */
	public void setFullScreen(final boolean setFull) {
		if (setFull != m_isFullScreen) {
			// stop processing window events now
			m_processWindowEvents = false;
			boolean wasVisible = isVisible();

			setVisible(false);
			dispose();

			if (setFull) {
				Toolkit.getDefaultToolkit().addAWTEventListener(
						scheduleHideMouse, AWTEvent.MOUSE_MOTION_EVENT_MASK);
				timeToHideMouse.start();
				getBounds(m_framePosition);
				setBounds(new java.awt.Rectangle(java.awt.Toolkit
						.getDefaultToolkit().getScreenSize()));
			} else {
				Toolkit.getDefaultToolkit().removeAWTEventListener(
						scheduleHideMouse);
				timeToHideMouse.stop();
				setCursor(Cursor.getDefaultCursor());
				setBounds(m_framePosition);
			}
			setAlwaysOnTop(setFull);
			setUndecorated(setFull);
			setResizable(!setFull);
			validate();
			setVisible(wasVisible);
			m_isFullScreen = setFull;
			// the events posted here are in front of
			// this control event
			EventQueue.invokeLater(new Runnable() {
				public void run() {
					m_processWindowEvents = true;
					repaint();
				}
			});
		}
	}

	/*
	 * Overriden to stop WindowEvents when disposing internally.
	 */
	protected void processWindowEvent(WindowEvent we) {
		if (m_processWindowEvents)
			super.processWindowEvent(we);
	}

	/**
	 * Test Menu display
	 * @param args
	 */
	public static void main(String args[]) {
		c_TbtJFrame = new TbtMainFrame();
	}
}
