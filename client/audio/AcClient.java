// This utility is an Audio Control client that provides buttons for sending
// TCP messages to the Audio Receiver Agent
//
// Run with 
// javaw -jar acClient.jar 
// or to see debug messages
// java -jar acClient.jar
//
// 2004 Aug  7 jhm original creation
// 2016 Jun 29 jhm converted messaging from UDP to TCP for smart remote 
//             functionality 

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

public class AcClient extends JFrame
                        implements ActionListener, WindowListener {
                      
    private static String agentHostname = "jimson";
    //private static final String agentHostname = "georgia";
    private static final int MEDIA_CONTROL_PORT = 2000;
    
    // computerSources hosts have a digital audio output link into the amplifier
    protected ArrayList<String> computerSources = 
        new ArrayList<String>(Arrays.asList("georgia","jimson","lvr-mm","den-mm"));
    // HTPC hosts default to front speakers on, rear speakers off    
    protected ArrayList<String> htpcSources = 
        new ArrayList<String>(Arrays.asList("lvr-mm","den-mm"));
    protected static boolean isSurroundOption;
    protected static boolean restoreReceiverStatePending=false;
    protected static String applicationName;
    protected RxClient rxClient;
    protected TcpConnection arAgent = new TcpConnection(agentHostname, MEDIA_CONTROL_PORT);
    String localHostname = GlobalUtilities.GetLocalHostname();
    
    protected ArrayList<JButton> fmButtons = new ArrayList<JButton>();
    protected ArrayList<JButton> inputButtons = new ArrayList<JButton>();
    protected ArrayList<JButton> surroundButtons = new ArrayList<JButton>();
    
    protected ArrayList<ArCmd> arCmdList = new ArrayList<ArCmd>();
    Container pane = getContentPane();
    GridBagLayout layout = new GridBagLayout();
    GridBagConstraints constraints = new GridBagConstraints();
    
    public AcClient(String applicationName) {
        
        super(applicationName);
        setSize(400, 500);
        
        pane.setLayout(layout);
        constraints.fill = GridBagConstraints.BOTH;  // enable x and y sizing
        
        if (isSurroundOption) {
            initSurroundPanel();
        } else {
            initAudioPanel();
        }        
          
        arCmdList.add(new ArCmd("register", arAgent));
        arCmdList.add(new ArCmd("deregister", arAgent));
        arCmdList.add(new ArCmd("reregister", arAgent));
        arCmdList.add(new ArCmd("rear speakers", arAgent));
        arCmdList.add(new ArCmd("front speakers", arAgent));

        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        addWindowListener(this);
        
        rxClient = new RxClient();
        rxClient.start();   // start the thread to receive messages

    }
    
    private void initAudioPanel() {
        setIconImage(Toolkit.getDefaultToolkit().getImage("audio.jpg"));

        createArCmdGrouped(0,  0, 1, 1, 1, 1, "KQED",  "fm1", fmButtons);     // A1 88.3
        createArCmdGrouped(1,  0, 1, 1, 1, 1, "89.1",  "fm2", fmButtons);     // A2
        createArCmdGrouped(2,  0, 1, 1, 1, 1, "89.9",  "fm3", fmButtons);     // A3
        createArCmdGrouped(3,  0, 1, 1, 1, 1, "KCRB",  "fm4", fmButtons);     // A4 90.9
        createArCmdGrouped(4,  0, 1, 1, 1, 1, "95.5",  "fm5", fmButtons);     // A5
        createArCmdGrouped(5,  0, 1, 1, 1, 1, "95.9",  "fm6", fmButtons);     // A6
        createArCmdGrouped(6,  0, 1, 1, 1, 1, "96.7",  "fm7", fmButtons);     // A7
        createArCmdGrouped(7,  0, 1, 1, 1, 1, "102.7", "fm8", fmButtons);     // A8
        
        createArCmdGrouped(0,  1, 2, 2, 1, 2, "Computer", localHostname, inputButtons);
        createArCmdGrouped(2,  1, 2, 2, 1, 2, "Radio", "fm", inputButtons);
        createArCmdGrouped(4,  1, 2, 2, 1, 2, "TV", "tv", inputButtons);
        createArCmdGrouped(6,  1, 2, 2, 1, 2, "DVD", "dvd", inputButtons);
        
          createArCmdOnOff(0,  3, 4, 2, 1, 2, "Computer Speakers", "rear speakers");
          createArCmdOnOff(4,  3, 4, 2, 1, 2, "Living Room Speakers", "front speakers");
          createArCmdOnOff(0,  5, 8, 2, 1, 2, "Den Speakers", "b speakers");
          
         createArCmdSlider(0,  7, 8, 2, 1, 2, "Volume", "master volume", false, 10, 100);
         
          createArCmdOnOff(0,  9, 8, 4, 4, 4, "Mute", "mute");
          createArCmdOnOff(0, 13, 8, 4, 4, 4, "Power", "audio power");
          
        // add commands to support an arAgent "restore state"
        arCmdList.add(new ArCmdEffectSlider(arSlider("center volume", 20), arAgent));
        arCmdList.add(new ArCmdEffectSlider(arSlider("surround volume", 20), arAgent));
        arCmdList.add(new ArCmdEffectSlider(arSlider("subwoofer volume", 20), arAgent));
        createArCmdGrouped("Min", "surround min", surroundButtons);
        createArCmdGrouped("Std", "surround std", surroundButtons);
        createArCmdGrouped("Max", "surround max", surroundButtons);
        createArCmdGrouped("Video Aux", "video aux", inputButtons);
        createArCmdGrouped(computerSources, localHostname, inputButtons);
    }
    
    private void initSurroundPanel() {
        setIconImage(Toolkit.getDefaultToolkit().getImage("surround.jpg"));
        
        createArCmdGrouped(0,  0, 1, 1, 1, 1, "G", "georgia", inputButtons);
        createArCmdGrouped(1,  0, 1, 1, 1, 1, "J", "jimson", inputButtons);
        createArCmdGrouped(2,  0, 1, 1, 1, 1, "TV", "tv", inputButtons);
        createArCmdGrouped(3,  0, 1, 1, 1, 1, "DVD", "dvd", inputButtons);
        createArCmdGrouped(4,  0, 1, 1, 1, 1, "LMM", "lvr-mm", inputButtons);
        createArCmdGrouped(5,  0, 1, 1, 1, 1, "DMM", "den-mm", inputButtons);
        
        createArCmdGrouped(0,  1, 2, 2, 1, 1, "Min", "surround min", surroundButtons);
        createArCmdGrouped(2,  1, 2, 2, 1, 1, "Std", "surround std", surroundButtons);
        createArCmdGrouped(4,  1, 2, 2, 1, 1, "Max", "surround max", surroundButtons);
        
          createArCmdOnOff(0,  3, 2, 2, 1, 1, "CR Spkr", "rear speakers");
          createArCmdOnOff(2,  3, 2, 2, 1, 1, "LR Spkr", "front speakers");
          createArCmdOnOff(4,  3, 2, 2, 1, 1, "Den Spkr", "b speakers");
          
         createArCmdSlider(0,  5, 6, 3, 1, 1, "Master Volume", "master volume", false, 10, 100);
         createArCmdSlider(0,  8, 6, 3, 1, 1, "Center Volume", "center volume", true, 5, 20);
         createArCmdSlider(0, 11, 6, 3, 1, 1, "Surround Volume", "surround volume", true, 5, 20);
         createArCmdSlider(0, 14, 6, 3, 1, 1, "Subwoofer Volume", "subwoofer volume", true, 5, 20);
         
          createArCmdOnOff(0, 17, 3, 2, 1, 1, "Mute", "mute");
          createArCmdOnOff(3, 17, 3, 2, 1, 1, "Power", "audio power");
          
        // add commands to support an arAgent "restore state"
        createArCmdGrouped("Radio", "fm", inputButtons);
        createArCmdGrouped("Video Aux", "video aux", inputButtons);
    }       
    
    public static void main(String[] args) {
        isSurroundOption = (args.length>0 && args[0].equals("-s")) ? true : false;
        applicationName = 
            isSurroundOption ? "Surround Control" : "Audio Control";
        exitIfAlreadyRunning();
        JFrame frame = new AcClient(applicationName);
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension windowSize = frame.getSize();
        frame.setLocation(Math.max(0,(screenSize.width - windowSize.width)/2), 
        Math.max(0,(screenSize.height-windowSize.height)/2));
        frame.setVisible(true);
    }
    
    // add an ArCmd button to a group without adding it to the pane
    private void createArCmdGrouped(String buttonName,
                                    String cmdSz,
                                    ArrayList<JButton> buttonGroup) {
        JButton button = new JButton(buttonName);
        button.setActionCommand(cmdSz);
        buttonGroup.add(button);
        arCmdList.add(new ArCmdGrouped(button, buttonGroup, arAgent));
    }
    
    // add a set of buttons from a list with one exclusion and not in the pane
    private void createArCmdGrouped(ArrayList<String> buttonNames, 
                                    String excludeName,
                                    ArrayList<JButton> buttonGroup) {
        Iterator itr = buttonNames.iterator();
        while(itr.hasNext()) {
            String buttonName = (String) itr.next();
            if (!buttonName.equals(excludeName)) {
                createArCmdGrouped(buttonName, buttonName, buttonGroup);
            }
        }
    }    
                                     
    private void createArCmdGrouped(int gridX, 
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
        arCmdList.add(new ArCmdGrouped(button, buttonGroup, arAgent));
    }
    
    private void createArCmdOnOff(String buttonName,
                                  String cmdSz) {
        JButton button = new JButton(buttonName);
        button.setActionCommand(cmdSz);
        arCmdList.add(new ArCmdOnOff(button, arAgent));
    }    
                                            
    private void createArCmdOnOff(int gridX, 
                                  int gridY, 
                                  int gridWidth, 
                                  int gridHeight,
                                  int weightX, 
                                  int weightY, 
                                  String buttonName,
                                  String cmdSz) {
                              
        JButton button = addButton(gridX, gridY, gridWidth, gridHeight, 
                                   weightX, weightY, buttonName, cmdSz);
        arCmdList.add(new ArCmdOnOff(button, arAgent));
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
        constraints.gridheight= gridHeight;
        constraints.weightx = weightX;
        constraints.weighty = weightY;
        button.setActionCommand(cmdSz);
        button.addActionListener(this);
        pane.add(button, constraints);   
        return button;
    }
    
    private JSlider arSlider(String cmdSz, int rangeEnd) {    
        JSlider slider = new JSlider(JSlider.HORIZONTAL, 0, rangeEnd, 0);
	    slider.getAccessibleContext().setAccessibleName(cmdSz);
        return slider;
    }    
    
    private void createArCmdSlider(int gridX, 
                                   int gridY, 
                                   int gridWidth, 
                                   int gridHeight,
                                   int weightX, 
                                   int weightY,
                                   String sliderName,
                                   String cmdSz,
                                   boolean effectControl,
                                   int majorTickSpacing,
                                   int rangeEnd) {
                                   
        JSlider slider = arSlider(cmdSz, rangeEnd);                      
        slider.setMajorTickSpacing(majorTickSpacing);
        slider.setMinorTickSpacing(1);
        slider.setPaintTicks(true);
        slider.setPaintLabels(true);
        slider.addChangeListener(new SliderListener());
	    slider.setBorder(new TitledBorder(sliderName));
        slider.setEnabled(false);
        
        constraints.gridx = gridX;
        constraints.gridy = gridY;
        constraints.gridwidth = gridWidth;
        constraints.gridheight= gridHeight;
        constraints.weightx = weightX;
        constraints.weighty = weightY;
        pane.add(slider, constraints);   
        
        arCmdList.add(effectControl ? 
                      new ArCmdEffectSlider(slider, arAgent) :
                      new ArCmdSlider(slider, arAgent) );
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
    
    class SliderListener implements ChangeListener {
        public void stateChanged(ChangeEvent event) {
            JSlider volumeSlider = (JSlider) event.getSource();
            String cmdSz = volumeSlider.getAccessibleContext().getAccessibleName();
            ArCmd arCmd = findArCmd(cmdSz);
            if (!volumeSlider.getValueIsAdjusting()) {
                if (volumeSlider.getValue()!=Integer.valueOf(arCmd.operand)) { 
                    int newVolume = volumeSlider.getValue();
                    System.out.println("SliderListener::stateChanged adjusting " +
                       cmdSz + " from " + arCmd.operand + " to " + newVolume);
                    volumeSlider.setEnabled(false);
                    arCmd.send(newVolume);
                }    
            }    
        }
    }
    
    public void actionPerformed(ActionEvent event) {
        findArCmd(event.getActionCommand()).send();
    } 
    
    private ArCmd findArCmd(String cmd) {
        ArCmd arCmd;
        Iterator itr = arCmdList.iterator();
        // search for an exact match
        while (itr.hasNext()) {
            arCmd = (ArCmd) itr.next();
            if (arCmd.sz.equals(cmd)) {
                return arCmd;
            }     
        }
        
        // search for a fuzzy match (disregards a volume level or on/off suffix)
        itr = arCmdList.iterator();
        while (itr.hasNext()) {
            arCmd = (ArCmd) itr.next();
            if (cmd.contains(arCmd.sz)) {
                return arCmd;
            }     
        }
        // System.out.println("findArCmd not found: " + cmd);
        return null;
    }
    
    private void restoreReceiverGroup(ArrayList<JButton> buttonGroup) {
        JButton button;
        Iterator itr = buttonGroup.iterator();
        while (itr.hasNext()) {
            button = (JButton) itr.next();
            findArCmd(button.getActionCommand()).resend();
        }
    }    
    
    private void restoreReceiverState() {
        findArCmd("audio power").send("on");
        findArCmd("video aux").send();
        findArCmd("rear speakers").resend();
        findArCmd("front speakers").resend();
        findArCmd("b speakers").resend();
        findArCmd("master volume").resend();
        findArCmd("center volume").resend();
        findArCmd("surround volume").resend();
        findArCmd("subwoofer volume").resend();
        
        restoreReceiverGroup(surroundButtons);
        restoreReceiverGroup(fmButtons);
        restoreReceiverGroup(inputButtons);
        
        findArCmd("audio power").resend();
        findArCmd("mute").resend();
    }
    
    private void setEnabledDspControls(boolean state) {
        ArCmd arCmd;
        Iterator itr = arCmdList.iterator();
        while (itr.hasNext()) {
            arCmd = (ArCmd) itr.next();
            arCmd.setEnabledDspControl(state);
        }
    }
    
    private void arAgentClose() {
        if (!arAgent.socket.isClosed()) {
            try {
                System.out.print("Closing agent connection...");
                arAgent.socket.close();
                System.out.println("done");
                arAgent.wasConnected=true;
            } catch (IOException e) {
                GlobalUtilities.DialogAndStdErrMsg(
                   "Error", 
                   "AcClient::arAgentClose failed",
                   e.toString());
            }
        }
    }        
    
    private class RxClient extends Thread {
    
        String msg;
        boolean firstRegistration = false;
        ArCmd arCmd;
        
        public RxClient() {
            enableControls(false);    
            findArCmd("audio power").setEnabled(false);
        }
        
        private void enableControls(boolean state) {
            Iterator itr = arCmdList.iterator();
            ArCmd arCmd;
            while (itr.hasNext()) {
                arCmd = (ArCmd) itr.next();
                if (arCmd.label().equals("Power")) {
                    arCmd.setEnabled(true);
                } else if (arCmd.label().equals("Computer")) {
                    arCmd.setEnabled(isComputerSourceCandidate(localHostname) && state);
                } else {
                    arCmd.setEnabled(state);
                }
            }
        }                
        
        public void run() {
        
            System.out.println("RxClient running");
            
            while(true) {
                
                if (arAgent.isConnected()) {
            
                    try {
                        msg = arAgent.rx.readLine();
                    } catch (IOException e) {    
                        msg = "";
                    }   
                
                    if (msg!=null && msg.length()>0) {
                        // msg strings match the syntax defined in yamaha_codes.h
                        System.out.println("RxClient msg: " + 
                           (restoreReceiverStatePending &&
                            !msg.equals("restore state") ? "ignoring " : "") + msg);
                        
                        if (msg.equals("restore state")) {
                            restoreReceiverState();
                        } else if (restoreReceiverStatePending) {
                            // ignore initial notifications
                            if (msg.equals("dsp effect on"))
                               restoreReceiverStatePending = false;
                            continue;
                        }    
                        
                        arCmd = findArCmd(msg);
                        
                        if (arCmd!=null) {
                            arCmd.processAgentNotification(msg);
                        }
                            
                        if (msg.equals("audio power on")) {
                            if (firstRegistration &&
                                isComputerSourceCandidate(localHostname)) {
                                findArCmd(localHostname).send();
                                findArCmd("rear speakers").send(isHTPC() ? "off" : "on");
                                findArCmd("front speakers").send(isSurroundOption || isHTPC() ? "on" : "off");
                                ArCmdSlider mv = (ArCmdSlider) findArCmd("master volume");
                                if (mv.slider.getValue() > 40) {
                                    mv.send(40);
                                }    
                            }    
                            firstRegistration = false;
                            enableControls(true);
                        }
                        else if (msg.equals("enable controls")) {
                            enableControls(true);
                        }    
                        else if (msg.equals("audio power off")) {
                            enableControls(false);
                        }  
                        else if (msg.equals("first registration")) {
                            firstRegistration = true;
                        } else if (msg.equals("dsp effect off")) {
                            setEnabledDspControls(false);
                        } else if (msg.equals("dsp effect on")) {
                            setEnabledDspControls(true);
                        }           
                            
                    } else {
                        arAgent.close();
                        enableControls(false);
                        restoreReceiverStatePending = true;
                        findArCmd("audio power").setEnabled(false);
                        sleep(10000);
                    }    
                }
                else {
                    if (arAgent.connect()) {
                        findArCmd("audio power").setEnabled(true);
                        if (findArCmd("audio power").isButtonPressed()) {
                            enableControls(true);
                        } 
                        findArCmd(arAgent.wasConnected ? "reregister" : "register").send();
                    } else {    
                        sleep(10000);
                    }
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
                        
        private boolean isHTPC() {
            Iterator itr = htpcSources.iterator();
            while(itr.hasNext()) {
                if (itr.next().equals(localHostname)) {
                    return true;
                }
            }        
            return false;
        }    
        
        private void sleep(int ms) {
            try {
                Thread.sleep(ms);
            } catch (InterruptedException e) {}
        }        
    }            
    
    public void windowClosed (WindowEvent e) {}
    public void windowOpened (WindowEvent e) {}
    public void windowDeiconified (WindowEvent e) {}
    public void windowIconified (WindowEvent e) {}
    public void windowDeactivated (WindowEvent e) {}
    public void windowActivated (WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
        if (arAgent.isConnected()) {
            findArCmd("deregister").send();
        }    
        arAgent.close();
        System.exit(0);
    }
}


class TcpConnection {
    protected Socket socket = new Socket();
    protected InetSocketAddress addr;
    protected DataOutputStream tx;
    protected BufferedReader rx; 
    protected String host;
    protected int port;
    protected boolean wasConnected=false;
    
    public TcpConnection(String host, int port) {
        this.host = host;
        this.port = port;
        addr = new InetSocketAddress(host, port);
    }
    
    protected boolean connect() {
        socket = new Socket();
        try {
           socket.connect(addr);
           tx = new DataOutputStream(socket.getOutputStream());
           rx = new BufferedReader(new InputStreamReader(socket.getInputStream()));
           return true;
        } catch (IOException e) { 
           System.out.println("TcpConnection: server " + host + " refused connection request");
        }
        return false;
    }
    
    protected void close() {
        if (!socket.isClosed()) {
            try {
                System.out.print("Closing agent connection...");
                socket.close();
                System.out.println("done");
                wasConnected=true;
            } catch (IOException e) {
                GlobalUtilities.DialogAndStdErrMsg(
                   "Error", 
                   "TcpConnection::close failed",
                   e.toString());
            }
        }
    }   
    
    protected boolean isConnected() {
        return !(socket.isClosed() || socket.getRemoteSocketAddress() == null);
    }  
    
}       


class GlobalUtilities {
    public static void DialogAndStdErrMsg(String dialogTitle, 
                                          String dialogMsg, 
                                          String exceptionMsg) {
        System.err.println(dialogMsg + "\n " + exceptionMsg); 
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

class ArCmdOnOff extends ArCmd {

    private JButton button;

    public ArCmdOnOff(JButton button, TcpConnection arAgent) {
        super(button.getActionCommand(), arAgent);
        this.operand = "off";
        this.button = button;
        button.setBorder(borderOff);
    } 
    
    protected void setEnabled(boolean state) { button.setEnabled(state); }
    protected String label() { return button.getText(); }
    
    protected boolean isButtonPressed() { 
        return button.getBorder().equals(borderOn); 
    }
    
    protected void processAgentNotification(String msg) { 
        button.setBorder(msg.contains(sz + " on") ? borderOn : borderOff);
        operand = isButtonPressed() ? "on" : "off"; 
    }
    
    protected void resend() { super.send(); }
    
    protected void send() { 
        operand = isButtonPressed() ? "off" : "on";
        super.send();
    }
    
    protected void send(String operand) {
        this.operand = operand;
        super.send();
    }    
}

class ArCmdGrouped extends ArCmd {

    private JButton button;
    private Border border;
    protected ArrayList<JButton> group;

    public ArCmdGrouped(JButton button, ArrayList<JButton> group, TcpConnection arAgent) {
        super(button.getActionCommand(), arAgent);
        this.button = button;
        button.setBorder(borderOff);
        this.group = group;
    } 
    
    protected void processAgentNotification(String msg) { 
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

    protected void resend() {
        if (isButtonPressed()) {
            send();
        }
    }

    protected String label() { return button.getText(); }
    protected boolean isButtonPressed() { 
        return button.getBorder().equals(borderOn); }
    protected boolean isGrouped() { return true; }
    protected ArrayList<JButton> group() { return group; }
    protected void setEnabled(boolean state) { button.setEnabled(state); }
}        

class ArCmdSlider extends ArCmd {

    public JSlider slider;

    public ArCmdSlider(JSlider slider, TcpConnection arAgent) {
        super(slider.getAccessibleContext().getAccessibleName(), arAgent);
        this.slider = slider;
    }    
    
    protected void processAgentNotification(String msg) { 
        slider.setEnabled(true);
        operand = msg.substring(msg.lastIndexOf(' ') + 1);
        slider.setValue(Integer.valueOf(operand));
    }
        
    protected void resend() { send(); }
    
    protected void send(int volume) {
        operand = String.valueOf(volume);
        send();
    }

    protected void setEnabled(boolean state) { slider.setEnabled(state); }
}       


class ArCmdEffectSlider extends ArCmdSlider {
    protected boolean enabledOverride = true;
    public ArCmdEffectSlider(JSlider slider, TcpConnection arAgent) {
        super(slider, arAgent);
    }    
    protected void resend() { 
        if (enabledOverride) {
           send(); 
        }
    }    
    protected void setEnabled(boolean state) { slider.setEnabled(state && enabledOverride); }
    protected void setEnabledDspControl(boolean state) { 
       enabledOverride=state; 
       slider.setEnabled(state); 
    }
}    


class ArCmd {
    protected String sz;
    protected String operand;
    protected TcpConnection arAgent;
    protected static Border borderOn = BorderFactory.createLoweredBevelBorder();
    protected static Border borderOff = BorderFactory.createRaisedBevelBorder();
    
    public ArCmd(String sz, TcpConnection arAgent) {
        this.sz = sz;
        this.operand = "";
        this.arAgent = arAgent;
    }
    
    protected void send() {
        try {
            if (arAgent.socket.getRemoteSocketAddress() != null) {
                arAgent.tx.writeBytes(sz + 
                                      (operand.isEmpty() ? "": " ") + 
                                      operand + '\r');
                System.out.println("ArCmd: " + sz + " " + operand);
            }   
        }
        catch (IOException e) {    
            GlobalUtilities.DialogAndStdErrMsg(
                "Error",
                "ArCmd::send \"" + sz +"\" failed",
                e.toString()); 
        }
    }       
    
    protected void processAgentNotification(String msg) { 
        operand = msg.contains(sz + " on") ? "on" : "off"; 
    }
    
    protected boolean isButtonPressed() { return false; }
    protected boolean isGrouped() { return false; }
    protected ArrayList<JButton> group() { return null; }
    protected void resend() {}
    protected void setEnabled(boolean state) {}
    protected void setEnabledDspControl(boolean state) {}
    protected void send(int volume) {}
    protected void send(String operand) {}
    protected String label() { return ""; }
}    

