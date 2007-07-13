package tbtGui;
import java.awt.*;
import java.awt.image.*;
import java.awt.geom.AffineTransform;
import java.awt.event.*;

import javax.swing.*;

/**
 * The image used as background is 723x1054 pixels
 */

/**
 * @author Julien Merienne
 * Contains the game field
 */
public class Field extends JLabel implements MouseListener, MouseMotionListener {
	// this class is not supposed to be serialized, this field is declared only to get rid of
	// javac warnings
	private static final long serialVersionUID = 0;

	private static final int SCROLL_GAP = 40;
	
	private Timer repeatMoveField = new Timer(200, new FieldScroller());
	private boolean m_isMoving = false;
	private Point m_moveFieldDirection = new Point();
	
	private int m_currentDisplayMode = Game.STANDARD_DISPLAY;

	private JViewport m_fieldViewPort = null;

	private class FieldScroller implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			Point newViewPosition = m_fieldViewPort.getViewPosition();
			newViewPosition.translate(m_moveFieldDirection.x, m_moveFieldDirection.y);
			Rectangle r = new Rectangle(newViewPosition.x, newViewPosition.y, m_fieldViewPort.getSize().width, m_fieldViewPort.getSize().height);
	        scrollRectToVisible(r);
		}
	}

	public void mouseDragged(MouseEvent e) {
        //The user is dragging us, so scroll!
        Rectangle r = new Rectangle(e.getX(), e.getY(), 1, 1);
        scrollRectToVisible(r);
    }

	private void moveNorth()
	{
		m_moveFieldDirection.y = -SCROLL_GAP;
		
	}
	private void moveSouth()
	{
		m_moveFieldDirection.y = SCROLL_GAP;
	}
	private void moveEast()
	{
		m_moveFieldDirection.x = SCROLL_GAP;
	}
	private void moveWest()
	{
		m_moveFieldDirection.x = -SCROLL_GAP;
	}
/*	private void stopMoving()
	{
		m_moveFieldDirection.x = 0;
		m_moveFieldDirection.y = 0;
	}
*/
	public void mouseMoved(MouseEvent e) {
		// do nothing
		Point positionOnField = e.getPoint();
		Point viewPosition = m_fieldViewPort.getViewPosition();
		Point positionOnScrollPane = new Point(positionOnField.x - viewPosition.x,
				                               positionOnField.y - viewPosition.y);
		boolean moveField = false;

		if (isNearNorthEdge(positionOnScrollPane))
		{
			moveNorth();
			moveField = true;
		}
		if (isNearSouthEdge(positionOnScrollPane))
		{
			moveSouth();
			moveField = true;
		}
		if (isNearWestEdge(positionOnScrollPane))
		{
			moveWest();
			moveField = true;
		}
		if (isNearEastEdge(positionOnScrollPane))
		{
			moveEast();
			moveField = true;
		}
		
		if (moveField)
		{
			if (! m_isMoving)
			{
				repeatMoveField.setRepeats(true);
				repeatMoveField.restart();
				m_isMoving = true;
			}
		}
		else
		{
			if (m_isMoving)
			{
				repeatMoveField.stop();
				m_isMoving = false;
			}
		}
		
/*		
		Game.c_debug.debugLog(viewPosition + " => ViewPosition\n" +
				              positionOnScrollPane + " => position on scrollPane\n" +
				              positionOnField + " => positionOnField\n" +
				              m_fieldViewPort.getSize() + " => viewPort Size");
*/
	}
	
	private boolean isNearNorthEdge(Point cursorPosition)
	{
		if (cursorPosition.getY() < 10) return true;
		else return false;
	}
	private boolean isNearSouthEdge(Point cursorPosition)
	{
		if (cursorPosition.getY() > (m_fieldViewPort.getSize().getHeight() - 10))
			return true;
		else return false;
	}
	private boolean isNearWestEdge(Point cursorPosition)
	{
		if (cursorPosition.getX() < 10) return true;
		else return false;
	}
	private boolean isNearEastEdge(Point cursorPosition)
	{
		if (cursorPosition.getX() > (m_fieldViewPort.getSize().getWidth() - 10))
			return true;
		else return false;
	}

	public void mouseClicked(MouseEvent e) {
		// do nothing
	}


	public void mouseEntered(MouseEvent e) {
		// do nothing
	}


	public void mouseExited(MouseEvent e) {
	//	stopMoving();
	}


	public void mousePressed(MouseEvent e) {
		// do nothing
	}


	public void mouseReleased(MouseEvent e) {
		// do nothing
	}

	/**
	 * Set the container JViewport reference
	 * @param fieldViewPort : will be used to move the view in mouseMoved event
	 */
	public void setViewPort(JViewport fieldViewPort) {
		m_fieldViewPort = fieldViewPort;
	}

    /**
     * Select the display mode, either horizontal or vertical
     * @param displayMode : either Game.STANDARD_DISPLAY or Game.WIDE_DISPLAY
     */
    public void selectDisplayMode(int displayMode) {
    	if (m_currentDisplayMode == displayMode) return;
    	ImageIcon fieldImage = (ImageIcon) getIcon();
        int w = fieldImage.getIconWidth();
        int h = fieldImage.getIconHeight();
        int type = BufferedImage.TYPE_INT_RGB;  // other options, see api
        BufferedImage image = new BufferedImage(h, w, type);
        Graphics2D g2 = image.createGraphics();
    	int angle = 0;
    	
    	switch(displayMode)
    	{
    	case Game.STANDARD_DISPLAY:
    		angle = 90;
    		break;
    	case Game.WIDE_DISPLAY:
    		angle = -90;
    		break;
    	default:
    		return;
    	}

        double x = (h - w)/2.0;
        double y = (w - h)/2.0;
        AffineTransform at = AffineTransform.getTranslateInstance(x, y);
        at.rotate(Math.toRadians(angle), w/2.0, h/2.0);
        g2.drawImage(fieldImage.getImage(), at, this);
        g2.dispose();
        fieldImage = new ImageIcon(image);
        setIcon(fieldImage);
        
        m_currentDisplayMode = displayMode;
    }

	/**
	 * Generate a Field Panel
	 * @param backgroundImage : the background image to display in the field
	 */
	public Field(ImageIcon backgroundImage) {
        super(backgroundImage);

        addMouseMotionListener(this); //handle mouse drags
        addMouseListener(this); //handle mouse drags

		setOpaque(true); 
	}

}
