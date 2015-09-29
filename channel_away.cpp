#include <znc/main.h>
#include <znc/Modules.h>
#include <znc/User.h>
#include <znc/Chan.h>
#include <znc/IRCNetwork.h>


class CChannelAway: public CModule {
public:
	MODCONSTRUCTOR(CChannelAway) {}

	virtual ~CChannelAway() {}

	virtual EModRet OnChanMsg(CNick& Nick, CChan& Channel, CString& sMessage) {
		CString First = sMessage.Token(0).MakeLower();
		CString MyNick = m_pNetwork->GetIRCNick().GetNick().AsLower();
		if (m_pNetwork->IsIRCAway() && TalkingToMe(First, MyNick)) {
			PutIRC("PRIVMSG " + Channel.GetName() + " :" + Nick.GetNick() +
				": I am currently away. My timezone is PST." +
				"Please wait until I'm back or send me a message (I check them regularly)");
		}
		return CONTINUE;
	}

private:
	bool TalkingToMe(const CString &First, const CString &MyNick) const {
		return First == MyNick
			|| First == MyNick + ":"
			|| First == MyNick + ","
			|| First == CString("@") + MyNick
			|| First == CString("@") + MyNick + ":"
			|| First == CString("@") + MyNick + ",";
	}
};


template<> void TModInfo<CChannelAway>(CModInfo& Info) {
	Info.SetWikiPage("channel_away");
	Info.SetHasArgs(false);
}

USERMODULEDEFS(CChannelAway, "When away, autoreply to channels in which your nick is mentioned.")
