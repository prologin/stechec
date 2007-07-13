package tbtGui;

import java.awt.*;
import java.awt.event.*;
import javax.swing.border.*;
import javax.swing.*;

/**
 * 
 */

/**
 * @author Julien Merienne
 *
 */
public class DisplayJPanel extends JPanel {
	// this class is not supposed to be serialized, this field is declared only to get rid of
	// javac warnings
	private static final long serialVersionUID = 0;
	
	// The Menu screen
	public static final int MENU_SCREEN_ID = 1;
	private Menu m_menuScreen = null ;
	
	// The Game screen
	public static final int GAME_SCREEN_ID = 2;
	private Game m_gameScreen = null ;

	/**
	 * This is the JPanel in wich we display the different screens of the fame
	 * @param maxSize : the maximum size allowed for this container
	 */
	public DisplayJPanel(Dimension maxSize)
	{
		setLayout(new BorderLayout());

		m_menuScreen = new Menu(ImagePaths.MENU_IMAGE_0, maxSize);
 		m_gameScreen = new Game(ImagePaths.FIELD_IMAGE_0, ImagePaths.DASHBOARD_IMAGE_0, maxSize);
 		
 //		m_menuScreen.setBorder(new LineBorder(Color.BLUE, 1));//DEBUG
 //		m_gameScreen.setBorder(new LineBorder(Color.BLUE, 1));//DEBUG

		bindKeys();

		selectDisplay(GAME_SCREEN_ID);
	}

	private void bindKeys()
	{
		// DEBUG key bindings
		// Bind the 'F10' key event
		KeyStroke kf10 = KeyStroke.getKeyStroke(KeyEvent.VK_F10, 0, false);
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(kf10, "F10");
		getActionMap().put("F10", new DisplayGame());
		// Bind the 'F9' key event
		KeyStroke kf9 = KeyStroke.getKeyStroke(KeyEvent.VK_F9, 0, false);
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(kf9, "F9");
		getActionMap().put("F9", new DisplayMenu());
	}

	/**
	 * Change the displayed screen
	 * @param screenId : identify the screen to show, can be one of the following
	 *                   values : MENU_SCREEN_ID, GAME_SCREEN_ID, +++
	 */
	public void selectDisplay(int screenId)
	{
		JComponent screenToShow;
		switch (screenId)
		{
		case MENU_SCREEN_ID:
			screenToShow = m_menuScreen;
			break;
		case GAME_SCREEN_ID:
			screenToShow = m_gameScreen;
			break;
		default:
			return;
		}
		removeAll();
		add(screenToShow, BorderLayout.CENTER);
		setSize(screenToShow.getSize());
		TbtMainFrame.reDraw();
	}

//  DEBUG
	private class DisplayMenu extends AbstractAction {
		// this class is not supposed to be serialized, this field is declared only to get rid
		// of javac warnings
		private static final long serialVersionUID = 0;
		public void actionPerformed(ActionEvent e){
			selectDisplay(MENU_SCREEN_ID);
			TbtMainFrame.reDraw();
		}
	}
		 
	private class DisplayGame extends AbstractAction{
		// this class is not supposed to be serialized, this field is declared only to get rid
		// of javac warnings
		private static final long serialVersionUID = 0;
		public void actionPerformed(ActionEvent e){
			selectDisplay(GAME_SCREEN_ID);
			TbtMainFrame.reDraw();
		}
	}
//    DEBUG
}
