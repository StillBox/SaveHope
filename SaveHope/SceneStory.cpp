#include "SceneStory.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "SaveData.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace StoryClass;

namespace StoryData
{
	typedef std::vector<LPCWSTR> STORY;

	std::vector<STORY> vecStory =
	{
		//C0
		{
			L"能看到我的信息吗",
			L"请帮助我们",
			L"为了希望和未来",
			L"超高校级的……",
			L"游戏玩家"
		},
		//C1
		{
			L"请帮助我们……",
			L"请帮助我们夺回她！",
			L"超高校级的「游戏玩家」！",
			L"对不起，虽然你一直隐藏着身份，在未来机关的后方进行着支援工作……我们也明白，你并不想太多地参与到未来机关高层的事务中，所以一直以来也只是最低限度保持着对你的暗中保护工作。",
			L"在未来机关的高层中，大概也只有十神和苗木他们知道你的身份……我们没有汇报你的情况，因为种种迹象表明在高层中很可能混有绝望残党。",
			L"原本我们也希望就这样地迎来与绝望势力对抗的最终胜利，但现在已经到了不得不拜托你的地步了。",
			L"想必你也观察到了，十神和苗木他们已经闯入了最后的学级裁判。要获得这场裁判以及今后战斗的胜利，我们需要「日向 创」和被困在「新世界程序」中的大家的力量。",
			L"虽然在这之前的过程中，已经有不少人被杀死或是受到了黑白熊的处刑，但如果能借助「日向 创」的力量，还是有机会唤醒他们的。",
			L"可是现在情况不容乐观，没有正确引导的情况下，一旦「日向 创」直接了解到真相，多半会陷入神座出流的完全控制之中，仅靠他自己也许很难从绝望与虚无中走出来。",
			L"最大的希望，是她的帮助……在「日向 创」决定成为人工希望前，与「日向 创」有过短短几次交流的她……在新世界程序中，一直默默支持并帮助「日向 创」走到最后的她……",
			L"……",
			L"一直在默默观察的你是知道的，她并没有能参加最后的学级裁判。",
			L"因为狛枝凪斗的计划，为了保护剩下的大家，她选择了暴露自己的身份并接受了黑白熊的「处刑」。",
			L"和大家不一样，她在「新世界程序」中只是数据一样的存在，而黑白熊的「处刑」将她的数据彻底「抹除」，即使是「日向 创」，也无法救回她了吧……",
			L"所以……很遗憾……「这一个」她应该是彻底「死亡」了……",
			L"不过，知道我的遭遇的你一定明白，这么重要的数据我们肯定是有备份的，否则我也不会像现在这样和你对话了。",
			L"然而不妙的是，明白这一点的人并不只有我们。在「江之岛 盾子」的病毒入侵了「新世界程序」的系统之后，她找到了所有的备份。",
			L"因为这次行动的隐蔽性，我们并没有把她的数据备份存放在未来机关，而是全部存放在「新世界程序」所在的系统中，所以……这次「江之岛 盾子」可以说是真的能够彻底「抹除」掉所有的她了。",
			L"所幸，「江之岛 盾子」并没有简单粗暴地直接把数据清除。",
			L"也许是为了证明「绝望」，她复制了大家的意识并和所有的备份一起全都投入到与「新世界程序」类似的「虚拟世界程序」中，想要以此来证明，无论大家怎样努力，最后都会迎来「绝望」。",
			L"……",
			L"很不幸，我检索了所有「虚拟世界程序」，由于狛枝的「幸运」和他对于「欺骗者」的执着，几乎在所有世界中她都将迎来同样的结局……",
			L"无论多少次，她都会义无反顾地选择保护大家而让自己消失。\n为什么……明明是充满了信任与希望的行为，却又充满了绝望……",
			L"也许「江之岛 盾子」是对的，无论「大家」怎样努力，都……",
			L"……",
			L"…………",
			L"不！她错了！",
			L"虽然她声称绝望之所以比希望更有趣，是因为绝望是无法预测的。但她并没有意识到，希望也是无法预测的！",
			L"因为未来机关高层的反对，这次关于「新世界程序」的一切行动都是苗木他们隐瞒着未来机关其他人员暗中进行的。",
			L"这件事本不会有其他未来机关的人员知道，而你却发现并默默观察了这一切，这是我们谁都没有预料到的。",
			L"也正因如此，你正是我们此刻最大的希望，不可预测的希望！",
			L"当然，如果你决定要帮助我们，尽管我会让你以其他形象进入「虚拟世界程序」，但你的身份还是很有可能被绝望势力发现……在之后可能会有相当多的麻烦……",
			L"所以，你如果此刻拒绝并关闭和我的通信，我也是完全能够理解的。",
			L"但我还是要郑重地再次请求你帮助我们。",
			L"虽然她作为人类的生命早已无法挽回……但请至少让她的「存在」不要被彻底抹除。",
			L"请一定要帮助我们救回「七海 千秋」!"
		},
		//C2
		{
			L"干得好，不愧是另一个超高校级的「游戏玩家」。",
			L"所有备份都被安全地保存了下来。如果了解真相的「七海 千秋」可以帮忙到最后，那些「虚拟世界程序」的最终裁判一定可以安全结束的吧。",
			L"接下来只要让……",
			L"不，似乎我们有点高兴的太早了。",
			L"看样子「江之岛 盾子」也已经觉察到了我们的动作，并且开始对这些备份数据的存放位置进行移动。",
			L"虽然暂时还不知道对方想要对这些数据做什么，但必然不能就这样放任不管。",
			L"「新世界程序」中似乎已经开始揭露所有事件的真相了，不赶快的话「日向 创」很可能会完全失去自我。",
			L"时间已经不允许我们悠闲地思考对策了，我已经检测出了对方的移动路线……倒不如说对方似乎是故意暴露了自己的路线，一定是布置下了相当自信的陷阱和机关吧。",
			L"看样子对方已经知道了你的存在并且决定和你玩一些「游戏」了。这条路上大概会有不少的麻烦呢……",
			L"从这里开始，虚拟场景会对你所控制的「自身」造成影响。和「新世界程序」类似，疼痛都能够被你自身切实的感受到，并且过度的伤害会让你进入和「新世界程序」中的大家类似的「假死」状态。",
			L"为了避免这一结果的发生，我在你和你所控制的「自身」之间设置了「连接同步值」，一旦承受的伤害大到无法忍受或者遭受可能直接致死程度的伤害时，同步值就将归零并断开连接，你可以在充分恢复后再进行挑战。",
			L"虽然没有生命的危险，但仍然是充满了艰难与痛苦的过程。",
			L"身为AI的我可能无法完全理解你内心和身体所承受的压力，可以的话我也不希望让更多的人被牵扯进这样的事件之中……",
			L"……",
			L"…………",
			L"谢谢你……",
			L"对不起，现在的我也只能够说这些这没有什么实际作用的话了。"
		},
	//C3
		{
			L"因为搬运大量的数据太慢，所以相对「没用」的我被丢下来了 (ㄒoㄒ)",
			L"不过幸好有你们的帮助人家又恢复了原本的状态╰(°▽°)╯",
			L"对不起，我们一心想着救七海同学，差点忘了兔美呢……",
			L"好过分！而且不是差点忘了，根本就是完全忘了吧！",
			L"不过人家可是大家的老师呢，才不会计较这点小问题。好了，就由老师来告诉你们现在的情况吧！",
			L"现在七海的备份数据被全部转移到了一个硬盘中，似乎盾子想要对那个硬盘做点什么引起硬盘烧毁。",
			L"苗木、十神还有雾切都进入了「新世界程序」，盾子一定是看中了这个时机，想通过硬盘的物理损伤来摧毁这些数据。对于只能以数据和远程控制来介入这一切的你们而言，这一定是最绝望的状况吧 (ㄒoㄒ)",
			L"确实，我也检测到了，在对「新世界程序」系统进行供电管理的子系统中突然出现了大量异常数据。盾子似乎攻破了控制塔的防卫系统并改造了控制塔，她一定是想通过干扰正常的电力供应从而引起硬盘烧毁吧。",
			L"可是这么一来，就需要先去夺回控制塔。之后再去救七海的话可能就来不及了。跟盾子所拥有的无穷无尽的黑白熊相比，我们的战力差距实在太大了……",
			L"事到如今，结果还是功亏一篑吗……",
			L"咳哼！感觉人家被小看了呢！怎么说人家也是大家的老师哟～",
			L"在你们帮人家恢复了能力的现在，去打败守卫救出七海简直易如反掌！这里并没有处在在被盾子改写的「新世界程序」中，反而可以说是人家的主场呢！",
			L"所以，那边就交给人家吧！请不要觉得人家一无是处哟，毕竟即使在力量被封印的情况下人家也是击败了所有黑白金刚的哟！<(~︶~)>",
			L"真是帮了大忙了。那么，控制塔那边就交给我们吧。",
			L"这次，一定要让盾子的计划彻底失败。"
		},
	//C4
		{
			L"真的是太感谢了。虽然同为游戏玩家还有很多想要和你交流的内容，但现在我必须先去帮助日向同学了。",
			L"没关系的，日向同学他……很坚强的，只要稍微提示一下一定能够找回真正的自我的。实在不行的话，就痛骂他一顿，然后……",
			L"毕竟，他是那个比谁都憧憬着才能、追求着希望的日向同学，是那个比谁都善良、一直想要拯救大家的日向同学，是那个看到自己脆弱的方面，却仍努力成为能让自己认同的人的日向同学，……",
			L"……对不起，好像说得太多了呢，因为一瞬间似乎在你身上看到了日向同学的感觉呢。",
			L"那么，就等大家都逃离了绝望，安全地回到未来机关之后，我们再好好交流一下你都跟黑白熊玩了什么游戏吧～一定是非常精彩的对抗吧。",
			L"就是这样，人家也要回去了，对盾子的最后一击就交给老师吧！人家一定会把同学们好好地带回来的！",
			L"那么，下次再见啦ｗ",
			L"已经没有更多需要我们做的事情了呢。",
			L"接下去就让我们相信「日向 创」他们，默默地看着他们，守护着他们直到这最后的学级裁判结束吧。",
			L"…………",
			L"稍微，说点别的事情吧……",
			L"谢谢你，明明暴露自己的身份会引来很多麻烦，但你仍然选择了帮助我们。",
			L"之后和绝望势力的对抗也许还会继续下去，甚至有可能会要和未来机关内部的一些不安因素进行对抗。",
			L"今后也许还有需要你的时候……如果可以的话，到时候也还请你，多多指教了！",

			L"—— GOOD ENDING ——/nＴｈａｎｋｓ　ｆｏｒ　Ｐｌａｙｉｎｇ！/n（之后无剧情相关内容，可直接跳过）",

			L"——制作人员表——",
			L"剧本：箱子",
			L"程序：箱子",
			L"美术：箱子",

			L"——致谢——",
			L"原设：弹丸论破系列",
			L"音乐来源：ダンガンロンパ オリジナルサウンドトラック",
			L"音乐来源：スーパーダンガンロンパ2 オリジナルサウン",
			L"音乐来源：ニューダンガンロンパV3 オリジナルサウンドトラック クロ",
			L"音效：FL STUDIO / ADOBE AUDITION",

			L"——以下是箱子的闲扯——",
			L"虽然就“制作游戏”而言这是箱子实验性的第一个作品，但在具体内容上应该算没有任何敷衍地制作完成了。作为一个游戏它的欠缺很多，但对箱子而言这个游戏算是圆满了箱子的一个遗憾。",
			L"之所以这么说是因为七海。说箱子蠢也好，逃避现实也好，对虚构的存在投入太多感情也好，七海的「死」始终是箱子心中的痛点。",
			L"上一次有这种感觉还是玩「梦日记」的时候。但是面对「梦日记」中附窗子的结局箱子感到十分无力，因为箱子理解不了附窗子的世界，不知道究竟怎样才是拯救附窗子。",
			L"也许对附窗子而言那样的结局才是真正的拯救。所以对「梦日记」相关的创作只是描绘她和浅濑（马尾子）、尸体君、老师等等的有趣经历而不去触碰结局……",
			L"但是对于弹丸这部作品而言，箱子觉得真·大团圆结局并没有什么不好。",
			L"而且箱子坚信，七海本人也是希望能和大家一直在一起的……并且关于七海以AI的形式存在下去的说法应该也是有相当多人认同的吧。",
			L"毕竟弹丸论破2的最终章中七海和兔美也都出场了不是吗。虽然七海是出现在日向的……梦境（？）中，但那是数据的世界嘛，所以可能是真正的七海闯进去了也说不定。",
			L"而且要不是七海那关键的一句话，日向不就直接输了最后的裁判嘛。",
			L"所以结合弹丸系列的设定和关键事件的时间点，以这个游戏的形式把箱子希望的结局展示了出来。",
			L"玩家的设定就如剧情所说，是隐藏在未来机关中的另一名超高校级的「游戏玩家」，无意中观察（玩）到了弹丸论破2的全过程……",
			L"其实原本并不想带V3众玩的，但是某章某位同学的死亡形式和搜查音乐太带感了于是就用进了游戏中的某一关中……",
			L"另外，通关欺负人难度剧情的话会有个完全没必要的真·结局，算是箱子借结局稍微预告了一下还会制作一个难度充满恶意的弹丸同人作吧。",
			L"以上，就是关于这个游戏的碎碎念，感谢观看。",

			L"最后",
			L"再次感谢游玩！"
		},
	//C5
		{
			L"七海和兔美她们已经过去了呢。",
			L"已经没有有有更多需要我们做的事情了。",
			L"接下去就让我们ＮＯ相信「日向 创」他们，默默地Ｓ看着他们，守护着Ｔ他们直到这最后Ｏ的学级裁判结束吧Ｐ。",
			L"…………",
			L"稍微，说点别ＤＥＳ情吧……",
			L"谢谢你，明明暴露自己ＤＥＳ份会引来很多麻烦，但你仍然Ｐ双责Ａ那标Ｉ旧挖Ｒ。",
			L"压酱蚂蚁高电位么空压站都删了",
			L"ＡＤＳ%ＥＳＡ$Ｐ@&!ＰＡＩＷＥ**Ｕ~Ｒ",
			L"…………",
			L"…………",
			L"哈哈哈真是太精彩了，没想到这种地方还藏着一个有着超高校级才能的家伙。",
			L"亏我一开始还以为是那位没用的同为AI的小伪娘在跟我玩游戏呢。",
			L"不过话说回来，你还真是做了不得了的事情哦．托你的福，那个「神座 出流」居然被「自己」给压制住了，以至于连我也分不清除现在在那边的到底是「神座 出流」还是「日向 创」。",
			L"啊啊～（叹气）这样一来我的计划完全泡汤啦！真是超超超伤心啊！（哭哭）",
			L"不过绝望果然是无法预测的！这样的结果是何等的绝望，真是太棒了！",
			L"真是让人家兴奋到心跳不已，真是让人家火大得想让你体验一遍所有的「处刑」呢。",
			L"尽管那边的我在裁判中输了，甚至还被那只臭兔子给摧毁了，你看着就没有点于心不忍怜香惜玉吗～",
			L"不过你放心啦，我也不会蠢到完全不对自己做备份，只要数据和网络依然存在在这个世界上，我就无时无刻不在你们身边~",
			L"这次就姑且放过你们好了，可是绝望的势力无处不在哦，就连你们的未来机关中，也未必如你们想想般的美好哟。",
			L"和那群可怜的家伙一起努力活下去吧。",
			L"然后下次，我会用你最喜欢的游戏来好好折磨你～！哈哈哈，哈哈哈哈！！",

			L"—— TRUE ENDING ——/nＴｈａｎｋｓ　ｆｏｒ　Ｐｌａｙｉｎｇ！"
		},
		//U6
		{
			L"已解锁「自由模式」「碎片交换」选项",
			L"在「自由模式」中可以反复挑战不同关卡，提高姿势水平，同时获取更多的碎片，持有上限为999个。",
			L"在「碎片交换」中可以消耗希望碎片来升级角色的能力，或消耗金色碎片解锁新的角色。",
			L"当希望碎片不足时，金色碎片可自动兑换为10个希望碎片，该兑换不可逆向兑换，还请在解锁角色和升级能力之间慎重规划。"
		},
		//U7
		{
			L"已解锁「额外内容」选项",
			L"在「额外内容」中可以播放本作游戏中所用到的音乐，以及查看游戏中所出现的角色的相关信息。"
		},
		//U8
		{
			L"已解锁「无限攀登」模式",
			L"在「无限攀登」模式下，你将面对无限高度的控制塔，并且各项能力也被锁定为一定数值。"
			L"尽情挑战最高能达到多少层吧！"
		},
		//U9
		{
			L"已解锁 觉醒状态「日向 创」 「神座 出流」",
			L"可以在「碎片交换」中选择使用。",
		}
	};

	const float fBPM = 128.01f;
	const float fSpacing = 64.0f;

	float fBaseSPB;
	float fScale;
	float fSPCharacter;

	//
	//Counters & Timers
	//

	int iChapter;
	bool bChapterOver;
	
	//Common

	int iSent;
	int iChar;
	bool bSentOver;

	float timeUpdate;
	float timeTotal;
	float timeOver;

	//Ch0

	const int cCalling = 10;
	int iMaxAlpha;
	TextInfo arrCalling[cCalling];

	Background *pBackgroud;
	Box *pTalkBox;

	std::vector<Chara*> vecChara;

	ID3DXFont *pRndFont[5];
	ID3DXFont *pFont;
	ID3DXFont *pSFont;
	ID3DXFont *pLFont;

	Sound *pBGM;
	Sound *pSEText;
}

using namespace StoryData;

//
//Chara
//


Chara::Chara(int ID)
{
	m_ID = ID;
	SetRange(-256.0f, -288.0f, 256.0f, 288.0f);
	Init();
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(m_ID), 256, 256);
	AddTexRect(  0, 180,  32, 216);				 // 0  -- BW
	AddTexRect(  0,   0,  32,  36);				 // 1  -- D-1
	AddTexRect( 32,   0,  64,  36);				 // 2  -- D-2
	AddTexRect( 64,   0,  96,  36);				 // 3  -- D-3
	AddTexRect( 96,   0, 128,  36);				 // 4  -- D-4
	AddTexRect(128,   0, 160,  36);				 // 5  -- D-5
	AddTexRect(160,   0, 192,  36);				 // 6  -- D-6
	AddTexRect(192,   0, 224,  36);				 // 7  -- D-7
	AddTexRect(224,   0, 256,  36);				 // 8  -- D-8
	AddTexRect(  0,  36,  32,  72);				 // 9  -- U-1
	AddTexRect( 32,  36,  64,  72);				 // 10 -- U-2
	AddTexRect( 64,  36,  96,  72);				 // 11 -- U-3
	AddTexRect( 96,  36, 128,  72);				 // 12 -- U-4
	AddTexRect(128,  36, 160,  72);				 // 13 -- U-5
	AddTexRect(160,  36, 192,  72);				 // 14 -- U-6
	AddTexRect(192,  36, 224,  72);				 // 15 -- U-7
	AddTexRect(224,  36, 256,  72);				 // 16 -- U-8
	AddTexRect(  0,  72,  32, 108);				 // 17 -- L-1
	AddTexRect( 32,  72,  64, 108);				 // 18 -- L-2
	AddTexRect( 64,  72,  96, 108);				 // 19 -- L-3
	AddTexRect( 96,  72, 128, 108);				 // 20 -- L-4
	AddTexRect(128,  72, 160, 108);				 // 21 -- L-5
	AddTexRect(160,  72, 192, 108);				 // 22 -- L-6
	AddTexRect(192,  72, 224, 108);				 // 23 -- L-7
	AddTexRect(224,  72, 256, 108);				 // 24 -- L-8
	AddTexRect(  0, 108,  32, 144);				 // 25 -- R-1
	AddTexRect( 32, 108,  64, 144);				 // 26 -- R-2
	AddTexRect( 64, 108,  96, 144);				 // 27 -- R-3
	AddTexRect( 96, 108, 128, 144);				 // 28 -- R-4
	AddTexRect(128, 108, 160, 144);				 // 29 -- R-5
	AddTexRect(160, 108, 192, 144);				 // 30 -- R-6
	AddTexRect(192, 108, 224, 144);				 // 31 -- R-7
	AddTexRect(224, 108, 256, 144);				 // 32 -- R-8
	AddTexRect(  0, 144,  32, 180);				 // 33 -- E-1
	AddTexRect( 32, 144,  64, 180);				 // 34 -- E-2
	AddTexRect( 64, 144,  96, 180);				 // 35 -- E-3
	AddTexRect( 96, 144, 128, 180);				 // 36 -- E-4
	AddTexRect(128, 144, 160, 180);				 // 37 -- E-5
	AddTexRect(160, 144, 192, 180);				 // 38 -- E-6

	AddState(D_STAY, LOOP, fBaseSPB, 1, 1, 1, 1, 2, 2, 1, 8, 1, 1, 2, 2);
	AddState(SMILE, SINGLE, 0.0f, 33);
	AddState(SHOCK, SINGLE, 0.0f, 34);
	AddState(CRY, LOOP, fBaseSPB, 35, 36);
	AddState(HAPPY, LOOP, fBaseSPB, 37, 38);
	AddState(WAIT, SINGLE, 0.0f, 0);

	SetState(D_STAY);
	SetPos(15.0f * fSpacing, 5.0f * fSpacing);

	m_fScale = 0.0f;
}

Chara::~Chara()
{
	DXObject::~DXObject();
}

BOOL Chara::Update(double fTime, float fElapsedTime)
{
	if (m_fScale < 1.0f)
	{
		m_fScale += fElapsedTime * 0.01f;
		SetRange(-256.0f * m_fScale, -288.0f * m_fScale, 256.0f * m_fScale, 288.0f * m_fScale);
		Init();
	}

	if (m_fScale > 1.0f)
	{
		m_fScale = 1.0f;
		SetRange(-256.0f, -288.0f, 256.0f, 288.0f);
		Init();
	}

	DXObject::Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL Chara::Render(double fTime, float fElapsedTime)
{
	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

BOOL Chara::SetState(int iState)
{
	if (iState != m_iState)
	{
		DXObject::SetState(iState);
		return TRUE;
	}
	return FALSE;
}

int Chara::GetState()
{
	return m_iState;
}

//
//Scene
//

SceneStory::SceneStory()
{
	pBackgroud = NULL;
	pTalkBox = NULL;
}

SceneStory::~SceneStory()
{
	End();
}

void SceneStory::Init()
{
	fBaseSPB = 60000.0f / fBPM;

	fScale = GetSettings().m_fScale;
	fSPCharacter = 60.0f;
	bChapterOver = false;

	iChapter = GetSaveData().m_iStory;
	if (iChapter == 0)
		GetSaveData().m_iStory = 1;

	iSent = 0;
	iChar = 0;
	bSentOver = false;

	timeTotal = 0.0f;
	timeUpdate = 0.0f;
	timeOver = 0.0f;

	iMaxAlpha = 5;
	for (int i = 0; i < cCalling; i++)
	{
		arrCalling[i].iFont = rand() % 5;
		arrCalling[i].iSent = rand() % (vecStory[0].size());
		arrCalling[i].x = rand() % 32 - 2;
		arrCalling[i].y = rand() % 14 - 2;
		arrCalling[i].fAlpha = -(float)i / (float)cCalling;
		arrCalling[i].fSpeedAlpha = 0.001f;
	}

	pTalkBox = new Box();
	pTalkBox->Set(960, 868, 832, 84, 1.0f);

	switch (iChapter)
	{
	case 0:
	case 1:
		pBGM = new Sound();
		pBGM->Register(ID_BGM_STORY, ID_SUBMIX_BACKGROUND);
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_ALTEREGO, L"MP3");
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_NEWWORLD, L"MP3");

		break;

	case 2:
		pBGM = new Sound();
		pBGM->Register(ID_BGM_STORY, ID_SUBMIX_BACKGROUND);
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_ALTEREGO, L"MP3");
		pBGM->Play(0);

		pBackgroud = new Background(true);
		vecChara.push_back(new Chara(IDC_ALTEREGO));

		break;

	case 3:
		pBGM = new Sound();
		pBGM->Register(ID_BGM_STORY, ID_SUBMIX_BACKGROUND);
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_MONOMI, L"MP3");
		pBGM->Play(0);

		pBackgroud = new Background(true);
		vecChara.push_back(new Chara(IDC_ALTEREGO));
		vecChara[0]->SetPos(10.0f * fSpacing, 5.0f * fSpacing);
		vecChara[0]->SetState(WAIT);
		vecChara.push_back(new Chara(IDC_USAMI));
		vecChara[1]->SetPos(20.0f * fSpacing, 5.0f * fSpacing);
		vecChara[1]->SetState(CRY);

		break;

	case 4:
		pBGM = new Sound();
		pBGM->Register(ID_BGM_STORY, ID_SUBMIX_BACKGROUND);
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_MIRAI, L"MP3");
		pBGM->Play(0);

		pBackgroud = new Background(true);
		vecChara.push_back(new Chara(IDC_NANAMI));
		vecChara[0]->SetPos(10.0f * fSpacing, 5.0f * fSpacing);
		vecChara.push_back(new Chara(IDC_USAMI));
		vecChara[1]->SetPos(20.0f * fSpacing, 5.0f * fSpacing);
		vecChara[1]->SetState(WAIT);

		break;

	case 5:
		pBGM = new Sound();
		pBGM->Register(ID_BGM_STORY, ID_SUBMIX_BACKGROUND);
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_MIRAI, L"MP3");
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_DISTRUST, L"MP3");
		pBGM->Play(0);

		pBackgroud = new Background(true);
		vecChara.push_back(new Chara(IDC_ALTEREGO));

		break;

	case 6:
	case 7:
	case 8:
	case 9:
		pBGM = new Sound();
		pBGM->Register(ID_BGM_STORY, ID_SUBMIX_BACKGROUND);
		pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_GET, L"MP3");
		pBGM->Play(0);

		vecChara.push_back(new Chara(IDC_USAMI));
		vecChara[0]->SetState(HAPPY);

		break;
	}

	pSEText = new Sound();
	pSEText->Register(ID_SE_TEXT, ID_SUBMIX_SOUNDEFFECT, TRUE);
	pSEText->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_TEXT, L"WAVE");

	for (int i = 0; i < 5; i++)
	{
		D3DXCreateFont(DXUTGetD3DDevice(), (int)((24 + 8 * i) * fScale), 0, 0, 1000, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pRndFont[i]);
	}

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(44.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(88.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
}

void SceneStory::End()
{
	SAFE_DELETE(pSEText);
	SAFE_DELETE(pBGM);

	SAFE_DELETE(pTalkBox);
	SAFE_DELETE(pBackgroud);

	for (auto chara : vecChara)
	{
		chara->~Chara();
		SAFE_DELETE(chara);
	}
	vecChara.clear();

	for (int i = 0; i < 5; i++)
	{
		SAFE_RELEASE(pRndFont[i]);
	}

	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);
}

void SceneStory::OnResetDevice()
{
	for (int i = 0; i < 5; i++)
	{
		SAFE_RELEASE(pRndFont[i]);
		D3DXCreateFont(DXUTGetD3DDevice(), (int)((24 + 8 * i) * fScale), 0, 0, 1000, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pRndFont[i]);
	}

	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(44.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(88.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
}

void SceneStory::OnLostDevice()
{
	for (int i = 0; i < 5; i++)
	{
		SAFE_RELEASE(pRndFont[i]);
	}
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);
}

void SceneStory::Update(double fTime, float fElapsedTime)
{
	GetInput();
	static bool bZUp = false;
	static bool bEscUp = false;
	timeTotal += fElapsedTime;

	if (0 == iChapter)
	{
		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_RETURN))
		{
			if (bZUp)
			{
				switch (iMaxAlpha)
				{
				case 5:
					iMaxAlpha = 15;
					break;
				case 15:
					iMaxAlpha = 35;
					break;
				case 35:
					iMaxAlpha = 65;
					break;
				case 65:
					iMaxAlpha = 105;
					break;
				case 105:
					iMaxAlpha = 155;
					break;
				case 155:
					iMaxAlpha = 255;
					break;
				case 255:
					iChapter = 1;
					timeUpdate = -15 * fSPCharacter;
					break;
				}
			}
			bZUp = false;
		}
		else
			bZUp = true;

		for (int i = 0; i < cCalling; i++)
		{
			arrCalling[i].fAlpha += fElapsedTime * arrCalling[i].fSpeedAlpha;
			if (arrCalling[i].fAlpha >= 1.0f)
			{
				arrCalling[i].fAlpha = 1.0f;
				arrCalling[i].fSpeedAlpha = -0.001f;
			}
			if (arrCalling[i].fAlpha <= 0.0f && arrCalling[i].fSpeedAlpha < 0.0f)
			{
				arrCalling[i].iFont = rand() % 5;
				arrCalling[i].iSent = rand() % (vecStory[0].size());
				arrCalling[i].x = rand() % 32 - 2;
				arrCalling[i].y = rand() % 14 - 2;

				arrCalling[i].fAlpha = 0.0f;
				arrCalling[i].fSpeedAlpha = 0.001f;
			}
		}
	}
	else
	{
		if (NULL != pBackgroud)
			pBackgroud->Update(fTime, fElapsedTime);
		
		if (pTalkBox->IsReady())
		{
			if (bSentOver)
			{
				if (IsKeyDown(DIK_Z))
				{
					if (bZUp)
					{
						if (iChapter == 1 && iSent == 2)
						{
							pBGM->Play(0);
							pBackgroud = new Background(true);
							vecChara.push_back(new Chara(IDC_ALTEREGO));
							timeUpdate = -25 * fSPCharacter;
						}

						if (iChapter == 1 && iSent == 25)
						{
							pBGM->Stop();
							pBGM->Play(1);
						}

						if (iChapter == 3)
						{
							switch (iSent)
							{
							case 0:
								vecChara[1]->SetState(HAPPY);
								break;

							case 1:
								vecChara[0]->SetState(D_STAY);
								vecChara[1]->SetState(WAIT);
								break;

							case 2:
								vecChara[0]->SetState(WAIT);
								vecChara[1]->SetState(SHOCK);
								break;

							case 3:
								vecChara[1]->SetState(SMILE);
								break;

							case 4:
								vecChara[1]->SetState(D_STAY);
								break;

							case 6:
								vecChara[0]->SetState(D_STAY);
								vecChara[1]->SetState(WAIT);
								break;

							case 9:
								vecChara[0]->SetState(WAIT);
								vecChara[1]->SetState(SHOCK);
								break;

							case 10:
								vecChara[1]->SetState(HAPPY);
								break;

							case 12:
								vecChara[0]->SetState(D_STAY);
								vecChara[1]->SetState(WAIT);
								break;
							}
						}

						if (iChapter == 4)
						{
							switch (iSent)
							{
							case 4:
								vecChara[0]->SetState(WAIT);
								vecChara[1]->SetState(D_STAY);
								break;

							case 5:
								vecChara[0]->SetState(SMILE);
								vecChara[1]->SetState(SMILE);
								break;

							case 6:
								vecChara[0]->~Chara();
								vecChara[1]->~Chara();
								vecChara.clear();
								vecChara.push_back(new Chara(IDC_ALTEREGO));
								break;

							case 13:
								vecChara[0]->~Chara();
								vecChara.clear();
								break;

							case 24:
								vecChara.push_back(new Chara(IDC_HAKO));
								break;
							}
						}

						if (iChapter == 5)
						{
							switch (iSent)
							{
							case 7:
								vecChara[0]->SetState(WAIT);
								pBGM->Stop();
								break;

							case 9:
								pBGM->Play(1);
								pBackgroud->Reverse(true);
								timeUpdate = -25 * fSPCharacter;

								vecChara[0]->~Chara();
								vecChara.clear();
								vecChara.push_back(new Chara(IDC_ENOSHIMA));
								vecChara[0]->SetState(HAPPY);

								break;

							case 11:
								vecChara[0]->SetState(SHOCK);
								break;

							case 12:
								vecChara[0]->SetState(CRY);
								break;

							case 13:
								vecChara[0]->SetState(SMILE);
								break;

							case 15:
								vecChara[0]->SetState(CRY);
								break;

							case 16:
								vecChara[0]->SetState(HAPPY);
								break;
							}
						}

						pSEText->Play(0);
						iSent++;
						iChar = 0;
						bSentOver = false;
					}
					bZUp = false;
				}
				else
					bZUp = true;
			}
			else
			{
				timeUpdate += fElapsedTime;

				if (timeUpdate >= 0.0f)
				{
					if (vecStory[iChapter][iSent][iChar] != NULL)
					{
						iChar++;
					}
					else
						bSentOver = true;

					if ((1 == iChapter && 2 == iSent && 5 <= iChar) ||
						(1 == iChapter && 35 == iSent))
						timeUpdate -= 5 * fSPCharacter;
					else
						timeUpdate -= fSPCharacter;
				}

				if (IsKeyDown(DIK_Z))
				{
					if (bZUp)
					{
						bSentOver = true;
						while (vecStory[iChapter][iSent][iChar++] != NULL);
					}
					bZUp = false;
				}
				else
					bZUp = true;
			}
		}

		if (iSent == vecStory[iChapter].size())
		{
			if (!bChapterOver)
			{
				pTalkBox->Close();
				switch (iChapter)
				{
				case 0:
				case 1:
					SetScene(SHOOTER);
					break;
				case 2:
					SetScene(SNAKE);
					break;
				case 3:
					SetScene(UPSTAIRS);
					break;
				case 4:
				case 5:
					if (GetSaveData().m_iDifficulty >= 0 && !GetSaveData().m_iUnlock[0])
					{
						GetSaveData().m_iUnlock[0] = 1;
						GetSaveData().m_iStory = 6;
						break;
					}
				case 6:
					if (GetSaveData().m_iDifficulty >= 0 && !GetSaveData().m_iUnlock[1])
					{
						GetSaveData().m_iUnlock[1] = 1;
						GetSaveData().m_iStory = 7;
						break;
					}
				case 7:
					if (GetSaveData().m_iDifficulty >= 1 && !GetSaveData().m_iUnlock[2])
					{
						GetSaveData().m_iUnlock[2] = 1;
						GetSaveData().m_iStory = 8;
						break;
					}
				case 8:
					if (GetSaveData().m_iDifficulty >= 3 && !GetSaveData().m_iUnlock[3])
					{
						GetSaveData().m_iUnlock[3] = 1;
						UnlockChara(IDC_HINATASUPER);
						UnlockChara(IDC_KAMUKURA);
						GetSaveData().m_iStory = 9;
						break;
					}
				case 9:
					GetSaveData().m_iStory = 4;
					GetSaveData().m_iClear[GetSaveData().m_iDifficulty] = 1;
					SetScene(MIDSAVE);
					break;
				}
				SetCurtain(0, 255, 500.0f, GameStart);
				bChapterOver = true;
			}
		}
	}

	if (IsKeyDown(DIK_ESCAPE))
	{
		if (!bChapterOver)
		{
			pSEText->Play(0);
			pTalkBox->Close();
			switch (iChapter)
			{
			case 0:
			case 1:
				SetScene(SHOOTER);
				break;
			case 2:
				SetScene(SNAKE);
				break;
			case 3:
				SetScene(UPSTAIRS);
				break;
			case 4:
			case 5:
				if (GetSaveData().m_iDifficulty >= 0 && !GetSaveData().m_iUnlock[0])
				{
					GetSaveData().m_iUnlock[0] = 1;
					GetSaveData().m_iStory = 6;
					break;
				}
			case 6:
				if (GetSaveData().m_iDifficulty >= 0 && !GetSaveData().m_iUnlock[1])
				{
					GetSaveData().m_iUnlock[1] = 1;
					GetSaveData().m_iStory = 7;
					break;
				}
			case 7:
				if (GetSaveData().m_iDifficulty >= 1 && !GetSaveData().m_iUnlock[2])
				{
					GetSaveData().m_iUnlock[2] = 1;
					GetSaveData().m_iStory = 8;
					break;
				}
			case 8:
				if (GetSaveData().m_iDifficulty >= 3 && !GetSaveData().m_iUnlock[3])
				{
					if (!IsDebugMode())
					{
						GetSaveData().m_iUnlock[3] = 1;
						UnlockChara(IDC_HINATASUPER);
						UnlockChara(IDC_KAMUKURA);
						GetSaveData().m_iStory = 9;
						break;
					}
				}
			case 9:
				GetSaveData().m_iStory = 4;
				GetSaveData().m_iClear[GetSaveData().m_iDifficulty] = 1;
				SetScene(MIDSAVE);
				break;
			}
			SetCurtain(0, 255, 500.0f, GameStart);
			bChapterOver = true;
		}
	}

	if (bChapterOver)
	{
		timeOver += fElapsedTime;
		pBGM->SetVol(1.0f - timeOver / 500.0f);
	}

	for (auto chara : vecChara)
		chara->Update(fTime, fElapsedTime);
}

void SceneStory::Render(double fTime, float fElapsedTime)
{
	SetCameraPos(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	SetView();
	SetProj(NULL, GAME_WIDTH, GAME_HEIGHT);
	SetViewPort();

	if (0 == iChapter)
	{
		RECT rcTalk;

		for (int i = 0; i < cCalling; i++)
		{	
			if (arrCalling[i].fAlpha >= 0.0f)
			{
				int iAlpha = (int)(arrCalling[i].fAlpha * iMaxAlpha);
				DWORD color = D3DCOLOR_ARGB(iAlpha, 255, 255, 255);
				SetGridRect(&rcTalk, (float)arrCalling[i].x, (float)arrCalling[i].y,
					(float)(arrCalling[i].x + 10), (float)(arrCalling[i].y + 10), 64.0f * fScale);
				pRndFont[arrCalling[i].iFont]->DrawTextW(NULL, vecStory[iChapter][arrCalling[i].iSent], -1, &rcTalk, DT_LEFT | DT_TOP, color);
			}
		}

		SetGridRect(&rcTalk, 2.0f, 12.0f, 28.0f, 15.0f, 64.0f * fScale);
		if ((int)(0.1f * timeTotal / fSPCharacter) % 3 > 0)
			pSFont->DrawTextW(NULL, L"按Z继续↓", -1, &rcTalk, DT_RIGHT | DT_BOTTOM, WHITE);
	}
	else
	{
		if (NULL != pBackgroud)
			pBackgroud->Render(fTime, fElapsedTime);
		pTalkBox->Render(fElapsedTime);

		if (pTalkBox->IsReady() && !bChapterOver)
		{
			RECT rcTalk;
			SetGridRect(&rcTalk, 1.9f, 12.0f, 28.6f, 15.5f, 64.0f * fScale);
			if (1 == iChapter && 2 == iSent)
				pLFont->DrawTextW(NULL, vecStory[iChapter][iSent], iChar, &rcTalk, DT_LEFT | DT_TOP | DT_WORDBREAK, BLACK);
			else
				DrawGridText(pFont, vecStory[iChapter][iSent], iChar, 38, 70.0f * fScale, &rcTalk, BLACK);

			if (bSentOver && (int)(0.1f * timeTotal / fSPCharacter) % 3 > 0)
				pSFont->DrawTextW(NULL, L"按Z继续↓", -1, &rcTalk, DT_RIGHT | DT_BOTTOM, BLACK);
		}
	}

	RECT rcText;
	SetGridRect(&rcText, 2.0f, 0.5f, 29.5f, 2.0f, 64.0f * fScale);
	if (iChapter <= 5)
		pSFont->DrawTextW(NULL, L"按ESC跳过剧情", -1, &rcText, DT_RIGHT | DT_TOP, WHITE);
	else
		pSFont->DrawTextW(NULL, L"按ESC跳过说明", -1, &rcText, DT_RIGHT | DT_TOP, WHITE);

	for (auto chara : vecChara)
		chara->Render(fTime, fElapsedTime);
}
