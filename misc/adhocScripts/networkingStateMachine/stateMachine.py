# big thanks to Brian Ray:
# https://medium.com/@brianray_7981/tutorial-write-a-finite-state-machine-to-parse-a-custom-language-in-pure-python-1c11ade9bd43

# this is my first FSM (in a high-level language) so it probably could be simpler 

# declare states for easier traversal
S_CLOSED = "CLOSED"
S_LISTEN = "LISTEN"
S_SYNSENT = "SYN_SENT"
S_SYNRCVD = "SYN_RCVD"
S_EST = "ESTABLISHED"
S_CLW = "CLOSE_WAIT"
S_CLOSING = "CLOSING"
S_FIN1 = "FIN_WAIT_1"
S_FIN2 = "FIN_WAIT_2"
S_LASTACK = "LAST_ACK"
S_TIMEWAIT = "TIME_WAIT"
S_ERROR = "ERROR"
    
# source, destination, conditions for every transition in state table...
FSM_MAP = (
    {'src': S_CLOSED,
        'dst': S_LISTEN,
        'condition': "APP_PASSIVE_OPEN"},
    {'src': S_CLOSED,
        'dst': S_SYNSENT,
        'condition': "APP_ACTIVE_OPEN"},   
        
    {'src': S_LISTEN,
        'dst': S_SYNRCVD,
        'condition': "RCV_SYN"},
    {'src': S_LISTEN,
        'dst': S_SYNSENT,
        'condition': "APP_SEND"},
    {'src': S_LISTEN,
        'dst': S_CLOSED,
        'condition': "APP_CLOSE"},

    {'src': S_SYNRCVD,
        'dst': S_FIN1,
        'condition': "APP_CLOSE"},
    {'src': S_SYNRCVD,
        'dst': S_EST,
        'condition': "RCV_ACK"},

    {'src': S_SYNSENT,
        'dst': S_SYNRCVD,
        'condition': "RCV_SYN"},
    {'src': S_SYNSENT,
        'dst': S_EST,
        'condition': "RCV_SYN_ACK"},
    {'src': S_SYNSENT,
        'dst': S_CLOSED,
        'condition': "APP_CLOSE"},

    {'src': S_EST,
        'dst': S_FIN1,
        'condition': "APP_CLOSE"},
    {'src': S_EST,
        'dst': S_CLW,
        'condition': "RCV_FIN"},
        
    {'src': S_FIN1,
        'dst': S_CLOSING,
        'condition': "RCV_FIN"},
    {'src': S_FIN1,
        'dst': S_TIMEWAIT,
        'condition': "RCV_FIN_ACK"},
    {'src': S_FIN1,
        'dst': S_FIN2,
        'condition': "RCV_ACK"},
        
    {'src': S_CLOSING,
        'dst': S_TIMEWAIT,
        'condition': "RCV_ACK"},
        
    {'src': S_FIN2,
        'dst': S_TIMEWAIT,
        'condition': "RCV_FIN"},

    {'src': S_TIMEWAIT,
        'dst': S_CLOSED,
        'condition': "APP_TIMEOUT"},

    {'src': S_CLW,
        'dst': S_LASTACK,
        'condition': "APP_CLOSE"},
        
    {'src': S_LASTACK,
        'dst': S_CLOSED,
        'condition': "RCV_ACK"},

)

class stateMachine(object):
    def __init__(self):
        self.state = S_CLOSED
    def handleText(self, text):
        frozen_state = self.state
        for transition in FSM_MAP:
            # find where current state and text input apply then update state. in a real FSM a callback function would also occur
            if (transition['src'] == frozen_state and transition['condition'] == text):
                self.state = transition['dst']
                print("{} -> {} : {}".format(text, frozen_state, self.state))
                return (True)
        # if the condition is not applicable:
        self.state = S_ERROR        
        return (False)

def traverse_TCP_states(events):
    print(events)
    myFSM = stateMachine()
    for i in events:
        myFSM.handleText(i)
    return myFSM.state