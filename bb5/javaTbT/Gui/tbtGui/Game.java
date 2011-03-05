package tbtGui;

import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
//import javax.swing.border.*;
import java.io.*;
import javax.swing.GroupLayout.*;

/**
 * 
 */

/**
 * @author Julien Merienne
 * This JPanel contains both the scrollable Field (a JLabel inside a JScrollPane) and
 * the game dashboard (a JPanel).
 * The field is displayed either vertically (STANDARD_DISPLAY) or
 *  horizontally (WIDE_DISPLAY)
 */
public class Game extends JPanel {
	// this class is not supposed to be serialized, this field is declared only to get rid of
	// javac warnings
	private static final long serialVersionUID = 0;

	// The field image size is 723x1054 and it can never be stretched
	// I have yet to know why but i need to add 3 pixels on both width and height in
	// order to avoid a small slide in the JScrollPane
	// ==> TODO : investigate the cause of this 3 pixels gap
	//
	// So for now, 726x1057
	//
	// The dashboard image size is 300x600 and it can be stretched
	//
	// STANDARD_DISPLAY : the field is displayed vertically, the maximum height is 1057 
	//                    the dashboard height is then stretched at 1057 and, to keep
	//                    the 1-2 ratio between width and height, the dashboard width
	//                    is streached at 528 => total width of 1254 (726+528)
	//
	// WIDE_DISPLAY     : the field is displayed horizontally, the maximum height is 726
	//                    the dashboard height is then stretched at 726 and, to keep
	//                    the 1-2 ratio between width and height, the dashboard width
	//                    is streached at 363 => total width of 1420 (1057+363)
	private static final int STANDARD_MAX_DISPLAY_WIDTH = 1254;
	private static final int STANDARD_MAX_DISPLAY_HEIGTH = 1057;
	private static final int WIDE_MAX_DISPLAY_WIDTH = 1420;
	private static final int WIDE_MAX_DISPLAY_HEIGTH = 726;

	public static final int STANDARD_DISPLAY = 1;
	public static final int WIDE_DISPLAY = 2;

	private int m_currentDisplayMode = 0;

	private Field m_field = null;
	private JScrollPane m_fieldScrollPane = null;
	private GameDashBoard m_dashBoard = null;

	public static GameDashBoard c_debug = null;

	private Dimension m_maxSize = null;
	
	/**
	 * @param fieldImagePath : path to the field image
	 * @param dashBoardImagePath : path to the dashboard image
	 * @param maxSize : the maximum size allowed for this container
	 */
	public Game(String fieldImagePath, String dashBoardImagePath, Dimension maxSize)
	{

        ImageIcon img = new ImageIcon(fieldImagePath);
		m_field = new Field(img);
		m_field.setSize(new Dimension(img.getIconWidth(),img.getIconHeight()));
        m_fieldScrollPane = new JScrollPane(m_field);
        m_field.setViewPort(m_fieldScrollPane.getViewport());
//        m_fieldScrollPane.setBorder(new LineBorder(Color.RED, 1));//DEBUG
        m_fieldScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        m_fieldScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		add(m_fieldScrollPane);

		m_dashBoard = new GameDashBoard(new File(dashBoardImagePath));
		c_debug = m_dashBoard;
		add(m_dashBoard);

		m_maxSize = maxSize;
		selectDisplayMode(WIDE_DISPLAY);
		System.out.println("m_fieldScrollPane : " + m_fieldScrollPane.getSize());
		System.out.println("m_field : " + m_field.getSize());
		bindKeys();
	}

	private void bindKeys()
	{
		// Bind the 'F8' key event
		KeyStroke kF8 = KeyStroke.getKeyStroke(KeyEvent.VK_F8, 0, false);
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(kF8, "F8");
		getActionMap().put("F8", new SwitchDisplayMode());
	}

	
	/**
	 * Choose a display mode
	 * @param displayMode : the display mode, either STANDARD_DISPLAY or WIDE_DISPLAY
	 */
	private void selectDisplayMode(int displayMode)
	{
		if (m_currentDisplayMode == displayMode) return;

		int gameDisplayWidth = 0;
		int gameDisplayHeight = 0;
		int fieldDisplayWidth = 0;
		int fieldDisplayHeight = 0;
		int dashBoardDisplayWidth = 0;
		int dashBoardDisplayHeight = 0;
		switch (displayMode)
		{
		case STANDARD_DISPLAY:
			gameDisplayWidth = STANDARD_MAX_DISPLAY_WIDTH;
			gameDisplayHeight = STANDARD_MAX_DISPLAY_HEIGTH;
			fieldDisplayWidth = 726;
			fieldDisplayHeight = 1057;
			break;
		case WIDE_DISPLAY:
			gameDisplayWidth = WIDE_MAX_DISPLAY_WIDTH;
			gameDisplayHeight = WIDE_MAX_DISPLAY_HEIGTH;
			fieldDisplayWidth = 1057;
			fieldDisplayHeight = 726;
			break;
		default:
			return;
		}
		// the game window must fit in the screen display
		if (gameDisplayWidth > m_maxSize.getWidth())
			gameDisplayWidth = (int) m_maxSize.getWidth();
		if (gameDisplayHeight > m_maxSize.getHeight())
			gameDisplayHeight = (int) m_maxSize.getHeight() ;
		// the field height must fit in the game display
		if (fieldDisplayHeight > gameDisplayHeight)
			fieldDisplayHeight = gameDisplayHeight ;
		// the dashboard height must be the same as the field height
		dashBoardDisplayHeight = fieldDisplayHeight;
		// the dashBoard Height/Witdh ratio is 2
		dashBoardDisplayWidth = dashBoardDisplayHeight/2;
		int freeWidth = gameDisplayWidth - dashBoardDisplayWidth;
		// the actual field width cannot be larger than gameWidth - dashbordWidth
		if (fieldDisplayWidth > freeWidth)
			fieldDisplayWidth = freeWidth;
		// so the game size is
		// fieldDisplayWidth+dashBoardDisplayWidth x fieldDisplayHeight+dashBoardDisplayHeight
		setSize(fieldDisplayWidth + dashBoardDisplayWidth,
				fieldDisplayHeight);
		m_field.selectDisplayMode(displayMode);

        m_dashBoard.setSize(new Dimension(dashBoardDisplayWidth, dashBoardDisplayHeight));
        m_fieldScrollPane.setSize(new Dimension(fieldDisplayWidth, fieldDisplayHeight));

		GroupLayout newLayout = new GroupLayout(this);
        setLayout(newLayout);

        SequentialGroup horizGroup = newLayout.createSequentialGroup();
        horizGroup.addComponent(m_fieldScrollPane, fieldDisplayWidth, fieldDisplayWidth, fieldDisplayWidth);
        horizGroup.addComponent(m_dashBoard, dashBoardDisplayWidth, dashBoardDisplayWidth, dashBoardDisplayWidth);
        newLayout.setHorizontalGroup(horizGroup);

        ParallelGroup vertGroup = newLayout.createParallelGroup(Alignment.LEADING);
        vertGroup.addComponent(m_fieldScrollPane, fieldDisplayHeight, fieldDisplayHeight, fieldDisplayHeight);
        vertGroup.addComponent(m_dashBoard, dashBoardDisplayHeight, dashBoardDisplayHeight, dashBoardDisplayHeight);
        newLayout.setVerticalGroup(vertGroup);

        m_currentDisplayMode = displayMode;

        System.out.println(" game.getSize() : " + getSize() );
        System.out.println(" field size :" + fieldDisplayWidth + " x " + fieldDisplayHeight);
        System.out.println(" db size :" + dashBoardDisplayWidth + " x " + dashBoardDisplayHeight);
        System.out.println(" m_fieldScrollPane.getSize() :" + m_fieldScrollPane.getSize());
        System.out.println(" m_dashBoard.getSize() :" + m_dashBoard.getSize());
	}

	// DEBUG
	private class SwitchDisplayMode extends AbstractAction {
		// this class is not supposed to be serialized, this field is declared only to get rid
		// of javac warnings
		private static final long serialVersionUID = 0;
		public void actionPerformed(ActionEvent e){
			if (m_currentDisplayMode == WIDE_DISPLAY)
				selectDisplayMode(STANDARD_DISPLAY);
			else
				selectDisplayMode(WIDE_DISPLAY);
			TbtMainFrame.reDraw();
		}
	}
}
