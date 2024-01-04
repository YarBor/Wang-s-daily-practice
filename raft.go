package raft

//
// this is an outline of the API that raft must expose to
// the service (or tester). see comments below for
// each of these functions for more details.
//
// rf = Make(...)
//   create a new Raft server.
// rf.Start(command interface{}) (index, term, isleader)
//   start agreement on a new log entry
// rf.GetState() (term, isLeader)
//   ask a Raft for its current term, and whether it thinks it is leader
// ApplyMsg
//   each time a new entry is committed to the log, each Raft peer
//   should send an ApplyMsg to the service (or tester)
//   in the same server.
//

import (
	//	"bytes"
	"context"
	"fmt"
	"log"
	"math/rand"
	"os"
	"sync"
	"sync/atomic"
	"time"

	//	"6.5840/labgob"
	"6.5840/labrpc"
)

var (
	LevelLeader    = int32(3)
	LevelCandidate = int32(2)
	LevelFollower  = int32(1)

	rpcTimeOut       = 50 * time.Millisecond
	HeartbeatTimeout = 25 * time.Millisecond
	voteTimeOut      = 100

	LogCheckBegin  = 0
	LogCheckAppend = 1
	LogCheckStore  = 2
	LogCheckIgnore = 3
	LogCheckReLoad = 4

	LogStateNormal      = int32(0)
	LogUpdateIng        = int32(1)
	pMsgStore           *MsgStore // nil
	pMsgStoreCreateLock = sync.Mutex{}
)

// as each Raft peer becomes aware that successive log entries are
// committed, the peer should send an ApplyMsg to the service (or
// tester) on the same server, via the applyCh passed to Make(). set
// CommandValid to true to indicate that the ApplyMsg contains a newly
// committed log entry.
//
// in part 2D you'll want to send other kinds of messages (e.g.,
// snapshots) on the applyCh, but set CommandValid to false for these
// other uses.
func (r *Raft) dolog(index int, i ...interface{}) {
	if index == -1 {
		log.Println(append([]interface{}{interface{}(fmt.Sprintf("{Level:%d}[T:%d]Server[%d]-[nil]", atomic.LoadInt32(&r.level), atomic.LoadInt32(&r.term), r.me))}, i...)...)
	} else {
		log.Println(append([]interface{}{interface{}(fmt.Sprintf("{Level:%d}[T:%d]Server[%d]-[%d]", atomic.LoadInt32(&r.level), atomic.LoadInt32(&r.term), r.me, index))}, i...)...)
	}
}

type RequestArgs struct {
	// Your data here (2A, 2B).
	Term      int32
	LogIndex  int32
	Time      time.Time
	SelfIndex int32

	// If there is this field,
	// it means that this request will use to
	// synchronize logs.
	Msg []*LogData
}
type RequestReply struct {
	// Your data here (2A).
	PeerTerm     int32
	PeerLogIndex int32
	ReturnTime   time.Time
	IsAgree      bool

	// If there is this field,
	// it means that this reply will use to
	// request more LogData.
	LogDataMsg *LogData
}

// save in Sequence table
type ApplyMsg struct {
	CommandValid bool
	Command      interface{}
	CommandIndex int
	CommandTerm  int

	// For 2D:
	SnapshotValid bool
	Snapshot      []byte
	SnapshotTerm  int
	SnapshotIndex int
}

// a pieces of Server Talk
type LogData struct {
	Msg           *ApplyMsg
	LastTimeIndex int
	LastTimeTerm  int
	SelfIndex     int
}

func (a *ApplyMsg) string() string {
	return fmt.Sprintf("%+v", *a)
}
func (a *LogData) string() string {
	return fmt.Sprintf("%+v", *a)
}

// a piece of Raft
type Log struct {
	Msgs  []*ApplyMsg
	MsgMu sync.Mutex
}

// A Go object implementing a single Raft peer.
type RaftPeer struct {
	C              *labrpc.ClientEnd
	mode           int32
	BeginHeartBeat chan struct{}
	StopHeartBeat  chan struct{}
}

// tmp stuct to update Log data
type MsgStore struct {
	msgs  []*LogData
	owner int
	mu    sync.Mutex
}

func (s *MsgStore) string() string {
	var str []byte
	for _, ld := range s.msgs {
		str = append(str, []byte(ld.string())...)
		str = append(str, []byte("\n\t")...)
	}
	str = append(str, []byte(fmt.Sprintf("\n\towner: %d", s.owner))...)
	return string(str)
}

type Raft struct {
	mu        sync.Mutex          // Lock to protect shared access to this peer's state
	peers     []*labrpc.ClientEnd // RPC end points of all peers
	persister *Persister          // Object to hold this peer's persisted state
	me        int                 // this peer's index into peers[]
	dead      int32               // set by Kill()

	isLeaderAlive int32 //
	level         int32 //
	// logIndex      int32 //
	// logIndexLock  sync.Locker

	term            int32 //
	termLock        sync.Locker
	timeOutChan     chan struct{}
	levelChangeChan chan struct{}
	raftPeers       []RaftPeer
	// Your data here (2A, 2B, 2C).
	commandLog Log
	// Look at the paper's Figure 2 for a description of what
	// state a Raft server must maintain.
}

func (r *Raft) getLogIndex() int32 {
	// r.logIndexLock.Lock()
	// defer r.logIndexLock.Unlock()
	// return r.logIndex
	r.commandLog.MsgMu.Lock()
	defer r.commandLog.MsgMu.Unlock()
	return int32(r.commandLog.Msgs[len(r.commandLog.Msgs)-1].CommandIndex)
}
func (r *Raft) getLogIndexNoMutex() int32 {
	// r.logIndexLock.Lock()
	// defer r.logIndexLock.Unlock()
	// return r.logIndex
	// r.commandLog.MsgMu.Lock()
	// defer r.commandLog.MsgMu.Unlock()
	return int32(r.commandLog.Msgs[len(r.commandLog.Msgs)-1].CommandIndex)
}

// func (r *Raft) LoadLogIndex(i int32) {
// r.logIndexLock.Lock()
// defer r.logIndexLock.Unlock()
// r.logIndex = i
// }
func (r *Raft) getTerm() int32 {
	r.termLock.Lock()
	defer r.termLock.Unlock()
	return r.term
}
func (r *Raft) setTerm(i int32) {
	r.termLock.Lock()
	defer r.termLock.Unlock()
	r.term = i
}
func (r *Raft) beginSendHeartBeat() {
	for i := range r.raftPeers {
		if i != r.me {
			select {
			case r.raftPeers[i].BeginHeartBeat <- struct{}{}:
			default:
			}
		}
	}
}
func (r *Raft) stopSendHeartBeat() {
	for i := range r.raftPeers {
		if i != r.me {
			select {
			case r.raftPeers[i].StopHeartBeat <- struct{}{}:
			default:
			}
		}
	}
}
func (r *Raft) registeHeartBeat(index int) {
	arg := RequestArgs{SelfIndex: int32(r.me)}
	for !r.killed() {
	restart:
		<-r.raftPeers[index].BeginHeartBeat
		for {
			arg.LogIndex = r.getLogIndex()
			arg.Term = atomic.LoadInt32(&r.term)
			arg.Time = time.Now()
			arg.Msg = nil

			rpl := RequestReply{}
			select {
			case <-r.raftPeers[index].StopHeartBeat:
				goto restart
			case <-time.After(HeartbeatTimeout):
				if r.getLevel() != LevelLeader {
					goto restart
				}
				ok := r.call(index, "Raft.HeartBeat", &arg, &rpl)
				// r.dolog(index, "Raft.HeartBeat", ok, arg.string(), rpl.string())
				if ok && !rpl.IsAgree && rpl.PeerLogIndex > r.getLogIndex() {
					r.dolog(index, "heartbeat return false Going to be Follower", arg.string(), rpl.string())
					r.changeToFollower(&rpl)
				}
			}
		}
	}
}
func (r *Raft) changeToLeader() {
	r.dolog(-1, "Going to Be Leader")
	// r.IsLeaderAlive = 1
	r.setLevel(LevelLeader)
	select {
	case r.levelChangeChan <- struct{}{}:
	default:
	}
	r.beginSendHeartBeat()
}
func (r *Raft) changeToCandidate() {
	r.dolog(-1, "Going to Be Candidate")
	// atomic.StoreInt32(&r.Level, LevelCandidate)
	r.setLevel(LevelCandidate)
	select {
	case r.levelChangeChan <- struct{}{}:
	default:
	}
}
func (r *Raft) changeToFollower(rpl *RequestReply) {
	r.dolog(-1, "Going to Be Follower")
	if atomic.LoadInt32(&r.level) == LevelLeader {
		r.stopSendHeartBeat()
	}
	if rpl != nil {
		if rpl.PeerTerm > r.getTerm() {
			r.setTerm(rpl.PeerTerm)
		}
	}
	r.setLevel(LevelFollower)
	select {
	case r.levelChangeChan <- struct{}{}:
	default:
	}
}

// the service or tester wants to create a Raft server. the ports
// of all the Raft servers (including this one) are in peers[]. this
// server's port is peers[me]. all the servers' peers[] arrays
// have the same order. persister is a place for this server to
// save its persistent state, and also initially holds the most
// recent saved state, if any. applyCh is a channel on which the
// tester or service expects Raft to send ApplyMsg messages.
// Make() must return quickly, so it should start goroutines
// for any long-running work.

func (r *Raft) HeartBeat(arg *RequestArgs, rpl *RequestReply) {
	r.dolog(int(arg.SelfIndex), "RECVED Heartbeat")
	defer func() {
		r.timeOutChan <- struct{}{}
	}()
	rpl.PeerLogIndex = r.getLogIndex()
	rpl.PeerTerm = r.getTerm()
	rpl.ReturnTime = time.Now()
	if arg.LogIndex < rpl.PeerLogIndex || arg.Term < rpl.PeerTerm {
		rpl.IsAgree = false
		return
	} else {
		rpl.IsAgree = true
	}
	if r.getLevel() == LevelLeader {
		rpll := rpl
		rpll.PeerTerm = arg.Term
		r.changeToFollower(rpll)
	}
	if arg.Msg == nil || len(arg.Msg) == 0 {
		return
	} else {
		for i := range arg.Msg {
			r.dolog(int(arg.SelfIndex), "Try LOAD-Log", arg.Msg[i].string())
		}
		rpl.LogDataMsg = r.updateMsgs(arg.Msg)
	}
}

// return currentTerm and whether this server
// believes it is the leader.
func (rf *Raft) GetState() (int, bool) {
	return int(rf.getTerm()), rf.getLevel() == LevelLeader
}

// save Raft's persistent state to stable storage,
// where it can later be retrieved after a crash and restart.
// see paper's Figure 2 for a description of what should be persistent.
// before you've implemented snapshots, you should pass nil as the
// second argument to persister.Save().
// after you've implemented snapshots, pass the current snapshot
// (or nil if there's not yet a snapshot).
func (rf *Raft) persist() {
	// Your code here (2C).
	// Example:
	// w := new(bytes.Buffer)
	// e := labgob.NewEncoder(w)
	// e.Encode(rf.xxx)
	// e.Encode(rf.yyy)
	// raftstate := w.Bytes()
	// rf.persister.Save(raftstate, nil)
}

// restore previously persisted state.
func (rf *Raft) readPersist(data []byte) {
	if data == nil || len(data) < 1 { // bootstrap without any state?
		return
	}
	// Your code here (2C).
	// Example:
	// r := bytes.NewBuffer(data)
	// d := labgob.NewDecoder(r)
	// var xxx
	// var yyy
	// if d.Decode(&xxx) != nil ||
	//    d.Decode(&yyy) != nil {
	//   error...
	// } else {
	//   rf.xxx = xxx
	//   rf.yyy = yyy
	// }
}

// the service says it has created a snapshot that has
// all info up to and including index. this means the
// service no longer needs the log through (and including)
// that index. Raft should now trim its log as much as possible.
func (rf *Raft) Snapshot(index int, snapshot []byte) {
	// Your code here (2D).

}

// example RequestVote RPC arguments structure.
// field names must start with capital letters!

func (r *RequestArgs) string() string {
	return fmt.Sprintf("%+v ", *r)
}

// example RequestVote RPC reply structure.
// field names must start with capital letters!

func (r *RequestReply) string() string {
	return fmt.Sprintf("%+v ", *r)
}

func (r *Raft) RequestPreVote(args *RequestArgs, reply *RequestReply) {
	reply.PeerLogIndex = r.getLogIndex()
	reply.PeerTerm = r.getTerm()
	reply.ReturnTime = time.Now()
	reply.IsAgree = atomic.LoadInt32(&r.isLeaderAlive) == 0
	r.dolog(int(args.SelfIndex), "get RequestPreVote REQUEST", args.string(), reply.string())
}

// example RequestVote RPC handler.
func (rf *Raft) RequestVote(args *RequestArgs, reply *RequestReply) {
	rf.termLock.Lock()
	defer rf.termLock.Unlock()
	reply.PeerTerm = rf.term
	rf.mu.Lock()
	defer rf.mu.Unlock()
	if rf.term < args.Term && args.LogIndex >= rf.getLogIndex() {
		reply.IsAgree = true
		rf.term = args.Term
		rf.timeOutChan <- struct{}{}
	} else {
		reply.IsAgree = false
	}
	reply.PeerLogIndex = rf.getLogIndex()
	reply.ReturnTime = time.Now()
	rf.dolog(int(args.SelfIndex), "Been Called RequestVote", args.string(), reply.string())
	// Your code here (2A, 2B).
}

// example code to send a RequestVote RPC to a server.
// server is the index of the target server in rf.peers[].
// expects RPC arguments in args.
// fills in *reply with RPC reply, so caller should
// pass &reply.
// the types of the args and reply passed to Call() must be
// the same as the types of the arguments declared in the
// handler function (including whether they are pointers).
//
// The labrpc package simulates a lossy network, in which servers
// may be unreachable, and in which requests and replies may be lost.
// Call() sends a request and waits for a reply. If a reply arrives
// within a timeout interval, Call() returns true; otherwise
// Call() returns false. Thus Call() may not return for a while.
// A false return can be caused by a dead server, a live server that
// can't be reached, a lost request, or a lost reply.
//
// Call() is guaranteed to return (perhaps after a delay) *except* if the
// handler function on the server side does not return.  Thus there
// is no need to implement your own timeouts around Call().
//
// look at the comments in ../labrpc/labrpc.go for more details.
//
// if you're having trouble getting RPC to work, check that you've
// capitalized all field names in structs passed over RPC, and
// that the caller passes the address of the reply struct with &, not
// the struct itself.
func (rf *Raft) sendRequestVote(server int, args *RequestArgs, reply *RequestReply) bool {
	ok := rf.peers[server].Call("Raft.RequestVote", args, reply)
	return ok
}

// the service using Raft (e.g. a k/v server) wants to start
// agreement on the next command to be appended to Raft's log. if this
// server isn't the leader, returns false. otherwise start the
// agreement and return immediately. there is no guarantee that this
// command will ever be committed to the Raft log, since the leader
// may fail or lose an election. even if the Raft instance has been killed,
// this function should return gracefully.
//
// the first return value is the index that the command will appear at
// if it's ever committed. the second return value is the current
// term. the third return value is true if this server believes it is
// the leader.

func (rf *Raft) checkMsg(msg *LogData) int {
	if msg == nil {
		return -1
	}
	rf.commandLog.MsgMu.Lock()
	defer rf.commandLog.MsgMu.Unlock()

	if len(rf.commandLog.Msgs) == 0 {
		return LogCheckBegin
	}
	lastRfLog := rf.commandLog.Msgs[len(rf.commandLog.Msgs)-1]

	rf.dolog(-1, "checking ", "\n\ttarget :", msg.string(), "\n len :", len(rf.commandLog.Msgs), "\n\tcache last", lastRfLog.string())
	switch {
	case msg.LastTimeIndex == -1 && msg.LastTimeTerm == -1:
		return LogCheckBegin
	case msg.LastTimeIndex == lastRfLog.CommandIndex:
		if msg.LastTimeTerm == lastRfLog.CommandTerm {
			return LogCheckAppend
		} else {
			return LogCheckReLoad
		}
	case msg.LastTimeIndex > lastRfLog.CommandIndex:
		return LogCheckStore
	case msg.LastTimeIndex < lastRfLog.CommandIndex:
		index := len(rf.commandLog.Msgs) - (lastRfLog.CommandIndex - msg.Msg.CommandIndex) - 1
		if index <= 0 {
			return LogCheckIgnore
		} else if rf.commandLog.Msgs[index].CommandTerm == msg.LastTimeTerm {
			return LogCheckIgnore
		} else if rf.commandLog.Msgs[index].CommandTerm != msg.LastTimeTerm {
			return LogCheckReLoad
		}
	default:
	}
	return -1
}
func (rf *Raft) appendMsg(msg *LogData) {
	if msg == nil || rf.getLevel() == LevelLeader {
		return
	}
	rf.commandLog.MsgMu.Lock()
	defer rf.commandLog.MsgMu.Unlock()
	// rf.logIndexLock.Lock()
	// defer rf.logIndexLock.Unlock()
	rf.commandLog.Msgs = append(rf.commandLog.Msgs, msg.Msg)
	rf.dolog(-1 , "appendMsg " , msg.string())
	// rf.logIndex++
}
func (rf *Raft) resetMsg(msg *LogData) {
	if msg == nil || rf.getLevel() == LevelLeader {
		return
	}
	rf.commandLog.MsgMu.Lock()
	defer rf.commandLog.MsgMu.Unlock()
	LastRfLog := rf.commandLog.Msgs[len(rf.commandLog.Msgs)-1]
	index := len(rf.commandLog.Msgs) - (LastRfLog.CommandIndex - msg.Msg.CommandIndex) - 1
	if index < 0 {
		return
	} else {
		rf.commandLog.Msgs[index] = msg.Msg
		rf.dolog(-1, "Log Reset", msg.Msg.string())
	}
}

func (m *MsgStore) insert(target *LogData) {
	if len(m.msgs) == 0 {
		m.msgs = []*LogData{target}
		return
	}
	if target.Msg.CommandIndex < m.msgs[0].Msg.CommandIndex {
		m.msgs = append([]*LogData{target}, m.msgs...)
		return
	}
	if target.Msg.CommandIndex > m.msgs[len(m.msgs)-1].Msg.CommandIndex {
		m.msgs = append(m.msgs, target)
		return
	}
	index := 0
	right := len(m.msgs)
	for index < right {
		mid := index + (right-index)/2
		if m.msgs[mid].Msg.CommandIndex < target.Msg.CommandIndex {
			index = mid + 1
		} else {
			right = mid
		}
	}
	if m.msgs[index].Msg.CommandIndex == target.Msg.CommandIndex {
		m.msgs[index] = target
	} else {
		m.msgs = append(m.msgs, nil)
		copy(m.msgs[index+1:], m.msgs[index:])
		m.msgs[index] = target
	}
}

// expect a orderly list in this
// when rf.msgs's tail  ==  this list 's head
// do update And Del this list 's head step by step
// to rf.msgs's tail != this list 's head || this list 's len == 0

func (rf *Raft) saveMsg(pMsgStore *MsgStore) *LogData {
	if pMsgStore == nil {
		return nil
	}
	rf.commandLog.MsgMu.Lock()
	defer rf.commandLog.MsgMu.Unlock()
	pMsgStore.mu.Lock()
	defer pMsgStore.mu.Unlock()
	if len(rf.commandLog.Msgs) == 0 {
		for i := range pMsgStore.msgs {
			rf.commandLog.Msgs = append(rf.commandLog.Msgs, pMsgStore.msgs[i].Msg)
			rf.dolog(-1, "Log", pMsgStore.msgs[i].string())
		}
		return nil
	}
	for tmpMsgStoreIndex := 0; tmpMsgStoreIndex < len(pMsgStore.msgs); tmpMsgStoreIndex++ {
		targetRfMsgsIndex := len(rf.commandLog.Msgs) - rf.commandLog.Msgs[len(rf.commandLog.Msgs)-1].CommandIndex + pMsgStore.msgs[tmpMsgStoreIndex].Msg.CommandIndex - 1
		if targetRfMsgsIndex <= 0 {
			pMsgStore.msgs = pMsgStore.msgs[1:]
			rf.dolog(-1, "Log OutOfBeginRange ", "Begin:>", rf.commandLog.Msgs[1].string(), " Request:>", pMsgStore.msgs[tmpMsgStoreIndex].string())
			continue
		}
		if targetRfMsgsIndex == len(rf.commandLog.Msgs) {
			rf.commandLog.Msgs = append(rf.commandLog.Msgs, pMsgStore.msgs[tmpMsgStoreIndex].Msg)
			rf.dolog(-1, "Log", *pMsgStore.msgs[tmpMsgStoreIndex])
			pMsgStore.msgs = pMsgStore.msgs[1:]
		}
		if targetRfMsgsIndex > len(rf.commandLog.Msgs) {
			rf.dolog(-1, "Log OutOfEndRange ", "End:>", rf.commandLog.Msgs[len(rf.commandLog.Msgs)-1].string(), " Request:>", pMsgStore.msgs[tmpMsgStoreIndex].string())
			break
		}
		if pMsgStore.msgs[tmpMsgStoreIndex].Msg.CommandIndex == rf.commandLog.Msgs[targetRfMsgsIndex].CommandIndex {
			if pMsgStore.msgs[tmpMsgStoreIndex].Msg.CommandTerm == rf.commandLog.Msgs[targetRfMsgsIndex].CommandTerm {
				continue
			} else {
				rf.dolog(-1, "Log Reset", "target:> ", rf.commandLog.Msgs[targetRfMsgsIndex].string(), "src :>", pMsgStore.msgs[tmpMsgStoreIndex].string())
				rf.commandLog.Msgs[targetRfMsgsIndex] = pMsgStore.msgs[tmpMsgStoreIndex].Msg
			}
			pMsgStore.msgs = pMsgStore.msgs[1:]
		}
	}
	if len(pMsgStore.msgs) >= 1 {
		return pMsgStore.msgs[0]
	} else {
		return nil
	}
}
func (rf *Raft) storeMsg(msg *LogData) {
	if msg == nil {
		return
	}
	if pMsgStore == nil || pMsgStore.owner != msg.SelfIndex {
		if pMsgStore != nil {
			rf.dolog(-1, "'pMsgStore' has been overwritten", "brfore Leader", pMsgStore.owner, "Now ", msg.SelfIndex)
		} else {
			rf.dolog(-1, "'pMsgStore' has been overwritten", "brfore Leader", nil, "Now ", msg.SelfIndex)
		}
		pMsgStoreCreateLock.Lock()
		defer pMsgStoreCreateLock.Unlock()
		if pMsgStore != nil {
			pMsgStore.mu.Lock()
			defer pMsgStore.mu.Unlock()
		}
		pMsgStore = &MsgStore{msgs: make([]*LogData, 0), owner: msg.SelfIndex, mu: sync.Mutex{}}
		return
	}
	pMsgStore.insert(msg)
}
func (rf *Raft) logBeginMsg(log *LogData) {
	rf.commandLog.MsgMu.Lock()
	defer rf.commandLog.MsgMu.Unlock()
	// rf.logIndexLock.Lock()
	// defer rf.logIndexLock.Unlock()
	if len(rf.commandLog.Msgs) == 1 {
		rf.commandLog.Msgs = append(rf.commandLog.Msgs, log.Msg)
		rf.dolog(-1, "LogBegin", log.Msg.string())
	} else {
		rf.commandLog.Msgs[1] = log.Msg
	}
	// rf.logIndex = int32(rf.commandLog.Msgs[len(rf.commandLog.Msgs)-1].CommandIndex)
}

// 期望index从高到低
func (rf *Raft) updateMsgs(msg []*LogData) *LogData {
	// 当对端发来的结构是 msg[0] == nil 时说明没有数据了
	for i := len(msg) - 1; i >= 0; i-- {
		result := rf.checkMsg(msg[i])
		switch result {
		case LogCheckBegin:
			rf.dolog(-1, "LogCheckBegin")
			rf.logBeginMsg(msg[i])
		case LogCheckAppend:
			rf.dolog(-1, "LogCheckAppend")
			rf.appendMsg(msg[i])
		case LogCheckIgnore:
			rf.dolog(-1, "LogCheckIgnore")
		case LogCheckReLoad:
			rf.dolog(-1, "LogCheckReLoad")
			rf.resetMsg(msg[i])
		case LogCheckStore:
			rf.dolog(-1, "LogCheckStore")
			rf.storeMsg(msg[i])
		default:
			pMsgStore.mu.Lock()
			defer pMsgStore.mu.Unlock()
			rf.dolog(-1, "The requested data is out of bounds ", pMsgStore.string(), "RequestIndex:>", pMsgStore.msgs[0].LastTimeIndex, "process will be killed")
			os.Exit(1)
		}
	}
	return rf.saveMsg(pMsgStore)
	// rf.MsgMu.Lock()
	// defer rf.MsgMu.Unlock()
	// // rf.LoadLogIndex()
	// rf.logIndexLock.Lock()
	// defer rf.logIndexLock.Unlock()
	// rf.logIndex++
	// rf.Msgs = append(rf.Msgs, &ApplyMsg{CommandValid: true, Command: command, CommandIndex: int(rf.logIndex)})
	// return int(rf.logIndex)
}
func (rf *Raft) Start(command interface{}) (int, int, bool) {
	// if rf.getLevel() == LevelLeader {
	// 	newIndex := rf.updateMsgs(command)
	// }
	if rf.getLevel() != LevelLeader {
		i, m, l := int(rf.getLogIndex()), int(rf.getTerm()), rf.getLevel() == LevelLeader
		rf.dolog(-1, "Start return ", "LogIndex", i, "Term", m, "Level", l)
		return i, m, l
	}

	var arg *RequestArgs
	func() {
		rf.commandLog.MsgMu.Lock()
		defer rf.commandLog.MsgMu.Unlock()

		rf.commandLog.Msgs = append(rf.commandLog.Msgs, &ApplyMsg{
			CommandTerm:  int(rf.getTerm()),
			CommandValid: true,
			Command:      command,
			CommandIndex: rf.commandLog.Msgs[len(rf.commandLog.Msgs)-1].CommandIndex + 1})

		// func() {
		// rf.logIndexLock.Lock()
		// defer rf.logIndexLock.Unlock()
		// rf.logIndex++
		// }()

		arg = &RequestArgs{
			Term:      rf.getTerm(),
			LogIndex:  rf.getLogIndexNoMutex(),
			Time:      time.Now(),
			SelfIndex: int32(rf.me),
			Msg: append(make([]*LogData, 0),
				&LogData{
					Msg:       rf.commandLog.Msgs[len(rf.commandLog.Msgs)-1],
					SelfIndex: rf.me}),
		}

		if len(rf.commandLog.Msgs)-2 < 0 {
			arg.Msg[0].LastTimeIndex = -1
			arg.Msg[0].LastTimeTerm = -1
		} else {
			arg.Msg[0].LastTimeIndex = rf.commandLog.Msgs[len(rf.commandLog.Msgs)-2].CommandIndex
			arg.Msg[0].LastTimeTerm = rf.commandLog.Msgs[len(rf.commandLog.Msgs)-2].CommandTerm
		}

	}()

	c := make(chan bool, len(rf.raftPeers)-1)
	for i := range rf.raftPeers {
		if i == rf.me {
			continue
		} else {
			go func(index int) {
				rpl := &RequestReply{}
				rf.dolog(index, "Raft.Heartbeat[LoadMsgBegin]", arg.string())
				ok := rf.call(index, "Raft.HeartBeat", arg, rpl)
				switch {
				case !ok:
					rf.dolog(index, "Raft.HeartBeat(sendMsg)", "Timeout")
				case !rpl.IsAgree:
					rf.dolog(index, "Raft.HeartBeat(sendMsg)", "Peer DisAgree", rpl.string())
				case rpl.LogDataMsg != nil:
					go rf.leaderUpdatePeer(index, rpl.LogDataMsg)
				default:
					c <- true
				}
				c <- false
			}(i)
		}
	}
	count := 0
	for i := 0; i < len(rf.raftPeers)-1; i++ {
		if <-c {
			count++
		}
		if count > len(rf.peers)-1 {
			break
		}
	}
	i, m, l := int(rf.getLogIndex()), int(rf.getTerm()), rf.getLevel() == LevelLeader
	rf.dolog(-1, "Start return ", "LogIndex", i, "Term", m, "Level", l)
	return i, m, l
	// Your code here (2B).

}

func (r *Raft) getLog(index int) (*ApplyMsg, *ApplyMsg) {
	r.commandLog.MsgMu.Lock()
	defer r.commandLog.MsgMu.Unlock()
	targetIndex := len(r.commandLog.Msgs) - r.commandLog.Msgs[len(r.commandLog.Msgs)-1].CommandIndex - index - 1
	if targetIndex-1 >= 0 && targetIndex < len(r.commandLog.Msgs)-1 {
		return r.commandLog.Msgs[targetIndex-1], r.commandLog.Msgs[targetIndex]
	}
	if targetIndex >= 0 && targetIndex < len(r.commandLog.Msgs)-1 {
		return nil, r.commandLog.Msgs[targetIndex]
	} else {
		return nil, nil
	}
}
func (r *Raft) leaderUpdatePeer(index int, msg *LogData) {
	if r.getLevel() != LevelLeader || atomic.LoadInt32(&r.raftPeers[index].mode) != LogUpdateIng {
		return
	} else {
		defer atomic.StoreInt32(&r.raftPeers[index].mode, LogStateNormal)
	}
	defer atomic.StoreInt32(&r.raftPeers[index].mode, LogStateNormal)
	// arg :=
	arg := &RequestArgs{
		Term:      r.getTerm(),
		LogIndex:  r.getLogIndex(),
		Time:      time.Now(),
		SelfIndex: int32(r.me),
		Msg:       make([]*LogData, 1),
	}
	for {
		if r.getLevel() != LevelLeader {
			return
		}
		preMsgNeedSend, MsgNeedSend := r.getLog(msg.LastTimeIndex)
		if MsgNeedSend != nil {
			arg.Msg[0] = &LogData{Msg: MsgNeedSend, SelfIndex: r.me}
			if preMsgNeedSend != nil {
				arg.Msg[0].LastTimeIndex = preMsgNeedSend.CommandIndex
				arg.Msg[0].LastTimeTerm = preMsgNeedSend.CommandTerm
			} else {
				arg.Msg[0].LastTimeIndex = -1
				arg.Msg[0].LastTimeTerm = -1
			}
		} else {
			arg.Msg[0] = nil
		}
		rpl := &RequestReply{}
		ok := r.call(index, "HeartBeat", arg, rpl)
		if !ok {
			r.dolog(index, "HeartBeat(Update) Timeout", arg.string())
			return
		} else if !rpl.IsAgree {
			r.dolog(index, "HeartBeat(Update) DisAgree", rpl.string())
			return
		} else if rpl.LogDataMsg != nil {
			msg = rpl.LogDataMsg
		} else {
			return
		}
	}
}

// the tester doesn't halt goroutines created by Raft after each test,
// but it does call the Kill() method. your code can use killed() to
// check whether Kill() has been called. the use of atomic avoids the
// need for a lock.
//
// the issue is that long-running goroutines use memory and may chew
// up CPU time, perhaps causing later tests to fail and generating
// confusing debug output. any goroutine with a long-running loop
// should call killed() to check whether it should stop.
func (rf *Raft) Kill() {
	atomic.StoreInt32(&rf.dead, 1)
	rf.dolog(-1, "killdead")
	// Your code here, if desired.
}

func (rf *Raft) killed() bool {
	z := atomic.LoadInt32(&rf.dead)
	return z == 1
}

func (rf *Raft) getLevel() int32 {
	return atomic.LoadInt32(&rf.level)
}

func (rf *Raft) setLevel(i int32) {
	atomic.StoreInt32(&rf.level, i)
	rf.dolog(-1, "Level set to ", i)
}

func (rf *Raft) ticker() {
	for !rf.killed() {
		switch rf.getLevel() {
		case LevelFollower:
			select {
			case <-rf.levelChangeChan:
			case <-rf.timeOutChan:
				atomic.StoreInt32(&rf.isLeaderAlive, 1)
			case <-time.NewTimer(time.Duration((int64(voteTimeOut) + rand.Int63()%150) * time.Hour.Milliseconds())).C:
				rf.dolog(-1, "TimeOut")
				atomic.StoreInt32(&rf.isLeaderAlive, 0)
				go tryToBecomeLeader(rf)
			}
		case LevelCandidate:
			select {
			case <-rf.levelChangeChan:
			case <-rf.timeOutChan:
			}
		case LevelLeader:
			select {
			case <-rf.levelChangeChan:
			case <-rf.timeOutChan:
			}
		}
	}
}

func tryToBecomeLeader(rf *Raft) {
	rf.changeToCandidate()
	arg := RequestArgs{Term: rf.getTerm() + 1, LogIndex: rf.getLogIndex(), Time: time.Now(), SelfIndex: int32(rf.me)}
	rpl := make([]RequestReply, len(rf.peers))
	wg := &sync.WaitGroup{}
	for i := range rf.raftPeers {
		if i != rf.me {
			wg.Add(1)
			go func(index int) {
				rf.dolog(index, "Raft.RequestPreVote  GO ", index, arg.string())
				// ok := rf.raftPeers[index].C.Call("Raft.RequestPreVote", &arg, &rpl[index])
				ok := rf.call(index, "Raft.RequestPreVote", &arg, &rpl[index])
				rf.dolog(index, "Raft.RequestPreVote RETURN ", index, ok, rpl[index].string())
				wg.Done()
			}(i)
		}
	}
	wg.Wait()
	count := 1
	for i := range rpl {
		if i != rf.me {
			if rpl[i].PeerTerm >= arg.Term || rpl[i].PeerLogIndex > rf.getLogIndex() {
				rf.changeToFollower(nil)
				return
			}
			if rpl[i].IsAgree {
				count++
			}
		}
	}
	if count > len(rf.peers)/2 {
		rf.setTerm(rf.getTerm() + 1)
	} else {
		rf.changeToFollower(nil)
		return
	}
	for i := range rf.raftPeers {
		if i != rf.me {
			wg.Add(1)
			go func(index int) {
				rf.dolog(index, "Raft.RequestVote", arg.string())
				// ok := rf.raftPeers[index].C.Call("Raft.RequestVote", &arg, &rpl[index])
				ok := rf.call(index, "Raft.RequestVote", &arg, &rpl[index])
				rf.dolog(index, "Raft.RequestVote", ok, rpl[index].string())
				wg.Done()
			}(i)
		}
	}
	wg.Wait()
	count = 1
	for i := range rpl {
		if i != rf.me {
			if rpl[i].PeerTerm >= rf.getTerm() || rpl[i].PeerLogIndex > rf.getLogIndex() {
				rf.changeToFollower(nil)
				return
			}
			if rpl[i].IsAgree {
				count++
			}
		}
	}
	if count < len(rf.peers)/2 {
		rf.changeToFollower(nil)
		return
	} else {
		rf.changeToLeader()
	}
}

func (r *Raft) call(index int, FuncName string, arg *RequestArgs, rpl *RequestReply) bool {
	// if r.getLevel() != LevelLeader {
	// r.dolog(-1, fmt.Sprintf("As %v tryto call %v", r.getLevel(), FuncName), arg.string(), rpl.string())
	// }
	ctx, cancel := context.WithTimeout(context.Background(), rpcTimeOut)
	defer cancel()
	i := false
	go func() {
		i = r.peers[index].Call(FuncName, arg, rpl)
		cancel()
	}()
	select {
	case <-ctx.Done():
	case <-time.After(rpcTimeOut):
		r.dolog(index, "Rpc Timeout ", FuncName, arg.string(), rpl.string())
	}
	return i
}

func Make(peers []*labrpc.ClientEnd, me int, persister *Persister, applyCh chan ApplyMsg) *Raft {
	rf := &Raft{}
	rf.peers = peers
	rf.persister = persister
	rf.me = me
	rf.level = LevelFollower
	rf.isLeaderAlive = 0
	// rf.logIndex = 0
	rf.term = 0
	rf.timeOutChan = make(chan struct{}, 1)
	rf.levelChangeChan = make(chan struct{}, 1)
	rf.raftPeers = make([]RaftPeer, len(rf.peers))
	for i := range rf.peers {
		if i != rf.me {
			rf.raftPeers[i] = RaftPeer{C: rf.peers[i], BeginHeartBeat: make(chan struct{}), StopHeartBeat: make(chan struct{})}
			rf.raftPeers[i].mode = LogStateNormal
			go rf.registeHeartBeat(i)
		}
	}
	// rf.logIndexLock = &sync.Mutex{}
	rf.termLock = &sync.Mutex{}
	file, _ := os.Create(fmt.Sprintf("/tmp/raft_%d.R", os.Getpid()))
	log.SetOutput(file)
	log.SetFlags(log.Lmicroseconds)

	rf.commandLog.Msgs = append(make([]*ApplyMsg, 0), &ApplyMsg{CommandValid: false, Command: nil, CommandIndex: 0, CommandTerm: -1, SnapshotValid: false, Snapshot: nil, SnapshotTerm: -1, SnapshotIndex: -1})
	rf.commandLog.MsgMu = sync.Mutex{}
	// Your initialization code here (2A, 2B, 2C).
	// initialize from state persisted before a crash
	rf.readPersist(persister.ReadRaftState())
	// start ticker goroutine to start elections
	go rf.ticker()

	return rf
}
