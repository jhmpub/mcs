// This utility is a gui client program that provides buttons for sending
// UDP messages to the server daemon acServer.exe.
//
// Run with 
// javaw -jar acClient.jar 
// or to see debug messages
// java -jar acClient.jar
//
// 2004 Aug  7 jhm original creation

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.Math;
import java.net.*;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.BorderFactory; 
import javax.swing.border.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class AcClient extends JFrame
                        implements ActionListener, WindowListener {
                      
    public static String serverName = "jimson";
    // computerSources hosts have a digital audio output link to the amplifier
    protected ArrayList<String> computerSources = 
        new ArrayList<String>(Arrays.asList("georgia","jimson","lvr-mm","den-mm","video aux"));
    protected static String applicationName = "Amplifier Control";
    
    protected RxClient rxClient;
    protected ArrayList<JButton> fmButtons = new ArrayList<JButton>();
    protected ArrayList<JButton> inputButtons = new ArrayList<JButton>();
    protected ArrayList<IrCmd> irCmdList = new ArrayList<IrCmd>();
    String localHostname;
    Container pane = getContentPane();
    GridBagLayout layout = new GridBagLayout();
    GridBagConstraints constraints = new GridBagConstraints();
    
    public AcClient() {
        
        super(applicationName);
        setSize(400, 500);
        
        initHostnames();
        
        pane.setLayout(layout);
        constraints.fill = GridBagConstraints.BOTH;  // enable x and y sizing
        createIrCmdGrouped(0,  0, 1, 1, 1, 1, "KQED",  "fm1", fmButtons);     // A1 88.3
        createIrCmdGrouped(1,  0, 1, 1, 1, 1, "89.1",  "fm2", fmButtons);     // A2
        createIrCmdGrouped(2,  0, 1, 1, 1, 1, "89.9",  "fm3", fmButtons);     // A3
        createIrCmdGrouped(3,  0, 1, 1, 1, 1, "KCRB",  "fm4", fmButtons);     // A4 90.9
        createIrCmdGrouped(4,  0, 1, 1, 1, 1, "95.5",  "fm5", fmButtons);     // A5
        createIrCmdGrouped(5,  0, 1, 1, 1, 1, "95.9",  "fm6", fmButtons);     // A6
        createIrCmdGrouped(6,  0, 1, 1, 1, 1, "96.7",  "fm7", fmButtons);     // A7
        createIrCmdGrouped(7,  0, 1, 1, 1, 1, "102.7", "fm8", fmButtons);     // A8
        createIrCmdGrouped(0,  1, 2, 2, 1, 2, "Computer", localHostname, inputButtons);
        createIrCmdGrouped(2,  1, 2, 2, 1, 2, "Radio", "fm", inputButtons);
        createIrCmdGrouped(4,  1, 2, 2, 1, 2, "TV", "tv", inputButtons);
        createIrCmdGrouped(6,  1, 2, 2, 1, 2, "DVD", "dvd", inputButtons);
          createIrCmdOnOff(0,  3, 4, 2, 1, 2, "Computer Speakers", "rear speakers");
          createIrCmdOnOff(4,  3, 4, 2, 1, 2, "Living Room Speakers", "front speakers");
          createIrCmdOnOff(0,  5, 8, 2, 1, 2, "Den Speakers", "channel b speakers");
         createIrCmdSlider(0,  7, 8, 2, 1, 2, "Volume", "master volume", 10, 100);
          createIrCmdOnOff(0,  9, 8, 4, 4, 4, "Mute", "mute");
          createIrCmdOnOff(0, 13, 8, 4, 4, 4, "Power", "power");
          
        irCmdList.add(new IrCmd("register"));
        irCmdList.add(new IrCmd("deregister"));
        
        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        addWindowListener(this);
        rxClient = new RxClient(serverName);
        rxClient.start();   // start the thread to receive messages
        findIrCmd("register").send();    // register with the server for the status of all controls
    }
    
    static final long serialVersionUID = 42L;
    
    public static void main(String[] args) {
        exitIfAlreadyRunning();
        JFrame frame = new AcClient();
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension windowSize = frame.getSize();
        frame.setIconImage(Toolkit.getDefaultToolkit().getImage("acIcon.jpg"));
        frame.setLocation(Math.max(0,(screenSize.width - windowSize.width)/2), 
        Math.max(0,(screenSize.height-windowSize.height)/2));
        frame.setVisible(true);
    }
    
    private void createIrCmdGrouped(int gridX, 
                                    int gridY, 
                                    int gridWidth, 
                                    int gridHeight,
                                    int weightX, 
                                    int weightY, 
                                    String buttonName,
                                    String cmdSz,
                                    ArrayList<JButton> buttonGroup) {
                              
        JButton button = addButton(gridX, gridY, gridWidth, gridHeight, 
                                   weightX, weightY, buttonName, cmdSz);
        buttonGroup.add(button);
        irCmdList.add(new IrCmdGrouped(button, buttonGroup));
    }
                                            
    private void createIrCmdOnOff(int gridX, 
                                  int gridY, 
                                  int gridWidth, 
                                  int gridHeight,
                                  int weightX, 
                                  int weightY, 
                                  String buttonName,
                                  String cmdSz) {
                              
        JButton button = addButton(gridX, gridY, gridWidth, gridHeight, 
                                   weightX, weightY, buttonName, cmdSz);
        irCmdList.add(new IrCmdOnOff(button, cmdSz + " on", "borderOn"));
        irCmdList.add(new IrCmdOnOff(button, cmdSz + " off", "borderOff"));
    }
    
    private JButton addButton(int gridX, 
                              int gridY, 
                              int gridWidth, 
                              int gridHeight,
                              int weightX, 
                              int weightY, 
                              String buttonName,
                              String cmdSz) {
        JButton button = new JButton(buttonName);
        constraints.gridx = gridX;
        constraints.gridy = gridY;
        constraints.gridwidth = gridWidth;
        constraints.weightx = weightX;
        constraints.weighty = weightY;
        button.setActionCommand(cmdSz);
        button.addActionListener(this);
        pane.add(button, constraints);   
        return button;
    }   
    
    private void createIrCmdSlider(int gridX, 
                                   int gridY, 
                                   int gridWidth, 
                                   int gridHeight,
                                   int weightX, 
                                   int weightY,
                                   String sliderName,
                                   String cmdSz,
                                   int majorTickSpacing,
                                   int rangeEnd) {
                              
        AcSlider slider = new AcSlider(JSlider.HORIZONTAL, 0, rangeEnd, 0);
	    slider.getAccessibleContext().setAccessibleName(cmdSz);
        slider.setMajorTickSpacing(majorTickSpacing);
        slider.setMinorTickSpacing(1);
        slider.setPaintTicks(true);
        slider.setPaintLabels(true);
        slider.addChangeListener(new SliderListener());
	    slider.setBorder(new TitledBorder(sliderName));
        
        constraints.gridx = gridX;
        constraints.gridy = gridY;
        constraints.gridwidth = gridWidth;
        constraints.weightx = weightX;
        constraints.weighty = weightY;
        pane.add(slider, constraints);   
        
        irCmdList.add(new IrCmdSlider(slider));
    }  
    
    
    // this function depends on an external executable, setForegroundWindow.exe
    // that must reside in the environment's search path
    private static void exitIfAlreadyRunning() {
    
        try {
            Process p = Runtime.getRuntime().exec(
                "setForegroundWindow.exe " + "\"" + applicationName + "\"");
            p.waitFor();                                      
            if (p.exitValue()==0) {                                      
                System.out.println(
                    "\nAcClient::exitIfAlreadyRunning: " + applicationName + 
                    " process found and set to foreground\n");
                System.exit(0);
            }   
        } catch (Exception e) {
            GlobalUtilities.DialogAndStdErrMsg(
                "Error", 
                "AcClient::exitIfAlreadyRunning: setForegroundWindow cmd failed",
                e.toString());
                System.exit(0);
        }                       
    }
    
    
    public void actionPerformed(ActionEvent event) {
    
        String cmdSz = event.getActionCommand();
        IrCmd irCmd = findIrCmd(cmdSz);
        
        if (irCmd.isGrouped()) {
            irCmd.send();
        } else {
            // send the command to toggle the state
            findIrCmd(cmdSz + (irCmd.isButtonPressed() ? 
                              " off":" on")).send();
        }    
    } 
    
    private void initHostnames() {
        localHostname = GlobalUtilities.GetLocalHostname();
        System.out.println("local hostname: " + localHostname);
        GlobalUtilities.SetServerName(serverName);
    }    
    
    private IrCmd findIrCmd(String cmdSz) {
        IrCmd irCmd;
        Iterator itr = irCmdList.iterator();
        // search for an exact match
        while (itr.hasNext()) {
            irCmd = (IrCmd) itr.next();
            if (irCmd.sz.equals(cmdSz)) {
                return irCmd;
            }     
        }
        
        // now search for a fuzzy match (disregards an on/off or volume level suffix)
        itr = irCmdList.iterator();
        while (itr.hasNext()) {
            irCmd = (IrCmd) itr.next();
            if (irCmd.sz.contains(cmdSz) || cmdSz.contains(irCmd.sz)) {
                return irCmd;
            }     
        }
        return null;
    }    
    
    class SliderListener implements ChangeListener {
        public void stateChanged(ChangeEvent event) {
            AcSlider slider = (AcSlider)event.getSource();
            String cmdSz = slider.getAccessibleContext().getAccessibleName();
            IrCmd irCmd = findIrCmd(cmdSz);
            if (!slider.getValueIsAdjusting()) {
                int newVolumeLevel = (int) slider.getValue();
                if (newVolumeLevel!=slider.volumeLevel) { 
                    System.out.println("SlideListener::stateChanged adjusting " +
                       cmdSz + " from " + slider.volumeLevel + " to " + 
                       newVolumeLevel);
                    slider.setEnabled(false);
                    irCmd.send(newVolumeLevel);
                }    
            }    
        }
    }
    
    private class RxClient extends Thread {
    
        static final int AMPLIFIER_CONTROL_NOTIFICATION_PORT = 2001;
        byte[] buf = new byte[256];
        String msg;
        boolean firstRegistration = false;
        boolean firstRegistrationFired = false;
        DatagramSocket socket;
        DatagramPacket packet;
        String rxHostname;
        InetSocketAddress serverAddr;
        IrCmd irCmd;
        
        public RxClient(String serverName) {
        
            try {
                socket = new DatagramSocket(AMPLIFIER_CONTROL_NOTIFICATION_PORT);
                packet = new DatagramPacket(buf, buf.length);
            }    
            catch (SocketException e) {    
                GlobalUtilities.DialogAndStdErrMsg(
                    "Error",
                    "RxClient: unable to bind to port " + 
                    AMPLIFIER_CONTROL_NOTIFICATION_PORT,
                    e.toString());
                System.exit(0);
            }   
             
            enableControls(false);    
            findIrCmd("power").setEnabled(false);
        }
        
        private void enableControls(boolean state) {
            Iterator itr = irCmdList.iterator();
            IrCmd irCmd;
            while (itr.hasNext()) {
                irCmd = (IrCmd) itr.next();
                if (irCmd.label().equals("Power")) {
                    irCmd.setEnabled(true);
                } else if (irCmd.label().equals("Computer")) {
                    irCmd.setEnabled(isComputerSourceCandidate(localHostname) && state);
                } else {
                    irCmd.setEnabled(state);
                }
            }
        }                
        
        public void run() {
        
            System.out.println("RxClient running");
            
            while(true) {
            
                try {
                    socket.receive(packet);
                } catch (IOException e) {    
                    GlobalUtilities.DialogAndStdErrMsg(
                        "Error",
                        "RxClient::run receive message failure",
                        e.toString());
                }    
                
                // msg strings match the syntax defined in yamaha_codes.h
                msg = new String(packet.getData(), 0, packet.getLength());    
                rxHostname = ((InetSocketAddress) 
                    packet.getSocketAddress()).getHostName().toLowerCase();
                System.out.println("RxClient msg received from " + 
                    rxHostname + ": " + msg);
                    
                if (msg.equals("power on")) {
                    if (firstRegistration && !firstRegistrationFired) {
                        if (isComputerSourceCandidate(localHostname)) {
                            findIrCmd(localHostname).send();
                            findIrCmd("rear speakers on").send();
                            findIrCmd("front speakers off").send();
                            IrCmdSlider mv = (IrCmdSlider) findIrCmd("master volume");
                            if (mv.slider.getValue() > 40) {
                                mv.send(40);
                            }    
                        }    
                        firstRegistration = false;
                        firstRegistrationFired = true;
                    } 
                    enableControls(true);
                }
                else if (msg.equals("enable controls")) {
                    enableControls(true);
                }    
                else if (msg.equals("power off")) {
                    enableControls(false);
                }  
                else if (msg.equals("first registration")) {
                    firstRegistration = true;
                }
                    
                irCmd = findIrCmd(computerSources.contains(msg) ? localHostname : msg);
                if (irCmd!=null) {
                    irCmd.processUpdate(msg);
                }    
            }
        }
        
        private boolean isComputerSourceCandidate(String hostname) {
            Iterator itr = computerSources.iterator();
            while(itr.hasNext()) {
                if (itr.next().equals(hostname)) {
                    return true;
                }
            }        
            return false;
        }    
    }            
    
    public void windowClosed (WindowEvent e) {}
    public void windowOpened (WindowEvent e) {}
    public void windowDeiconified (WindowEvent e) {}
    public void windowIconified (WindowEvent e) {}
    public void windowDeactivated (WindowEvent e) {}
    public void windowActivated (WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
        findIrCmd("deregister").send();
        System.exit(0);
    }
}


class GlobalUtilities {
    public static void DialogAndStdErrMsg(String dialogTitle, 
                                          String dialogMsg, 
                                          String exceptionMsg) {
        System.err.println(dialogMsg + " " + exceptionMsg); 
        MsgBox msgBox = MsgBox.msgBox(dialogTitle, dialogMsg);
        msgBox.requestFocus();
    }

    public static String GetLocalHostname() {
        try {
            return InetAddress.getLocalHost().getHostName();
        } catch (Exception e) {
            GlobalUtilities.DialogAndStdErrMsg(
               "Error",
               "GetLocalHostname: unable to get localhost name",
                e.toString());
                System.exit(0);
        }
        return "undefined";
    }    
    
    private static String serverName;
    public static void SetServerName(String name) { serverName=name; }
    public static String GetServerName() { return serverName; }
}    


class MsgBox extends Dialog implements ActionListener {
    boolean id = false;
    Button ok,can;
    
    public static MsgBox msgBox(String title, String msg) {
        return new MsgBox(new Frame(), title, msg);
    }    

    MsgBox(Frame frame, String title, String msg) {
        super(new Frame(), title, true);
        setLayout(new BorderLayout());
        add("Center",new Label(msg));
        addOKCancelPanel(false);
        createFrame();
        pack();
        setVisible(true);
    }

    void addOKCancelPanel(boolean okcan) {
        Panel p = new Panel();
        p.setLayout(new FlowLayout());
        createOKButton( p );
        if (okcan == true)
        createCancelButton( p );
        add("South",p);
    }

    void createOKButton(Panel p) {
        p.add(ok = new Button("OK"));
        ok.addActionListener(this); 
    }

    void createCancelButton(Panel p) {
        p.add(can = new Button("Cancel"));
        can.addActionListener(this);
    }

    void createFrame() {
        Dimension d = getToolkit().getScreenSize();
        setLocation(d.width/3,d.height/3);
    }

    public void actionPerformed(ActionEvent ae) {
        if(ae.getSource() == ok) {
            id = true;
            setVisible(false);
        }
        else if (ae.getSource() == can) {
            setVisible(false);
        }
    }
}


class IrCmdOnOff extends IrCmd {

    private JButton button;
    private Border border;

    public IrCmdOnOff(JButton button, String sz, String border) {
        super(sz);
        this.button = button;
        button.setBorder(borderOff);
        this.border=border.equals("borderOn") ? borderOn : borderOff;
    } 
    
    protected void processUpdate(String msg) { button.setBorder(border); }
    protected void setEnabled(boolean state) { button.setEnabled(state); }
    protected String label() { return button.getText(); }
    protected boolean isButtonPressed() { 
        return button.getBorder().equals(borderOn); }
}

class IrCmdGrouped extends IrCmd {

    private JButton button;
    private Border border;
    protected ArrayList<JButton> group;

    public IrCmdGrouped(JButton button, ArrayList<JButton> group) {
        super(button.getActionCommand());
        this.button = button;
        button.setBorder(borderOff);
        this.group = group;
    } 
    
    protected void processUpdate(String msg) { 
        Iterator itr = group.iterator();
        JButton button;
        while (itr.hasNext()) {
            button = (JButton) itr.next();
            if (msg.equals(button.getActionCommand())) {
                 button.setBorder(borderOn);
            } else {    
                 button.setBorder(borderOff);
            }
        }
    }
    protected void setEnabled(boolean state) { button.setEnabled(state); }
    protected String label() { return button.getText(); }
    protected boolean isButtonPressed() { 
        return button.getBorder().equals(borderOn); }
    protected boolean isGrouped() { return true; }
    protected ArrayList<JButton> group() { return group; }
}        

class IrCmdSlider extends IrCmd {

    public AcSlider slider;

    public IrCmdSlider(AcSlider slider) {
        super(slider.getAccessibleContext().getAccessibleName());
        this.slider = slider;
    }    
    
    protected void processUpdate(String msg) { 
        slider.setEnabled(true);
        slider.volumeLevel = Integer.valueOf(msg.substring(msg.lastIndexOf(' ')+1)).intValue();
        slider.setValue(slider.volumeLevel);
    }
        
    protected void setEnabled(boolean state) { slider.setEnabled(state); }
    
    protected void send(int volume) {
        datagram=super.makeDatagram(sz + " " + volume);
        super.send();
    }
}       

class IrCmd {

    protected String sz;
    private static final int AMPLIFIER_CONTROL_REQUEST_PORT = 2000;
    private static DatagramSocket socket=IrCmd.initSocket();
    private static InetAddress inetServer=IrCmd.initInetServer();
    protected static Border borderOn = BorderFactory.createLoweredBevelBorder();
    protected static Border borderOff = BorderFactory.createRaisedBevelBorder();
    protected DatagramPacket datagram;
    
    public IrCmd(String sz) {
        this.sz = sz;
        datagram = makeDatagram(sz);
    }    
    
    protected void send() {
        try {
            socket.send(datagram);
        }
        catch (IOException e) {    
            GlobalUtilities.DialogAndStdErrMsg(
                "Error",
                "IrCmd::sendCmd unable to send requested message",
                e.toString()); 
        }    
    }       
    
    protected boolean isButtonPressed() { return false; }
    protected boolean isGrouped() { return false; }
    protected ArrayList<JButton> group() { return null; }
    protected void processUpdate(String msg) {}
    protected void setEnabled(boolean state) {}
    protected void send(int volume) {}
    protected String label() { return ""; }
    
    protected DatagramPacket makeDatagram(String sz) {
        return new DatagramPacket(
            sz.getBytes(), 
            sz.length(), 
            inetServer, 
            AMPLIFIER_CONTROL_REQUEST_PORT);
    }
    
    public static DatagramSocket initSocket() {
        DatagramSocket ds=null;
        try {
            ds = new DatagramSocket();
        } catch (IOException e) {
            GlobalUtilities.DialogAndStdErrMsg(
                "Error",
                "IrCmd::initSocket initialization failure ",
                 e.toString()); 
            e.printStackTrace();
            System.exit(0);
        }
        return ds;    
    }    
        
    public static InetAddress initInetServer() {
        InetAddress ia=null;
        try {
            ia = InetAddress.getByName(GlobalUtilities.GetServerName());
        } catch (IOException e) {
            GlobalUtilities.DialogAndStdErrMsg(
                "Error",
                "IrCmd::initInetServer host " + GlobalUtilities.GetServerName() + " is unreachable ",
                 e.toString()); 
            e.printStackTrace();
            System.exit(0);
        } 
        return ia;   
    }    
}    


class AcSlider extends JSlider {
    protected int volumeLevel;
    public AcSlider(int orientation, int min, int max, int value) {
        super(orientation, min, max, value);
    }    
}    
