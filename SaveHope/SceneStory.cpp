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
			L"�ܿ����ҵ���Ϣ��",
			L"���������",
			L"Ϊ��ϣ����δ��",
			L"����У���ġ���",
			L"��Ϸ���"
		},
		//C1
		{
			L"��������ǡ���",
			L"��������Ƕ������",
			L"����У���ġ���Ϸ��ҡ���",
			L"�Բ�����Ȼ��һֱ��������ݣ���δ�����صĺ󷽽�����֧Ԯ������������Ҳ���ף��㲢����̫��ز��뵽δ�����ظ߲�������У�����һֱ����Ҳֻ������޶ȱ����Ŷ���İ��б���������",
			L"��δ�����صĸ߲��У����Ҳֻ��ʮ�����ľ����֪�������ݡ�������û�л㱨����������Ϊ���ּ�������ڸ߲��кܿ��ܻ��о����е���",
			L"ԭ������Ҳϣ����������ӭ������������Կ�������ʤ�����������Ѿ����˲��ò�������ĵز��ˡ�",
			L"�����Ҳ�۲쵽�ˣ�ʮ�����ľ�����Ѿ�����������ѧ�����С�Ҫ����ⳡ�����Լ����ս����ʤ����������Ҫ������ �����ͱ����ڡ�����������еĴ�ҵ�������",
			L"��Ȼ����֮ǰ�Ĺ����У��Ѿ��в����˱�ɱ�������ܵ��˺ڰ��ܵĴ��̣�������ܽ��������� �����������������л��ỽ�����ǵġ�",
			L"����������������ֹۣ�û����ȷ����������£�һ�������� ����ֱ���˽⵽���࣬��������������������ȫ����֮�У��������Լ�Ҳ����ѴӾ������������߳�����",
			L"����ϣ���������İ��������ڡ����� ����������Ϊ�˹�ϣ��ǰ���롸���� �����й��̶̼��ν�����������������������У�һֱĬĬ֧�ֲ����������� �����ߵ�����������",
			L"����",
			L"һֱ��ĬĬ�۲������֪���ģ�����û���ܲμ�����ѧ�����С�",
			L"��Ϊ��֦�M���ļƻ���Ϊ�˱���ʣ�µĴ�ң���ѡ���˱�¶�Լ�����ݲ������˺ڰ��ܵġ����̡���",
			L"�ʹ�Ҳ�һ�������ڡ������������ֻ������һ���Ĵ��ڣ����ڰ��ܵġ����̡����������ݳ��ס�Ĩ��������ʹ�ǡ����� ������Ҳ�޷��Ȼ����˰ɡ���",
			L"���ԡ������ź���������һ������Ӧ���ǳ��ס��������ˡ���",
			L"������֪���ҵ���������һ�����ף���ô��Ҫ���������ǿ϶����б��ݵģ�������Ҳ������������������Ի��ˡ�",
			L"Ȼ��������ǣ�������һ����˲���ֻ�����ǡ��ڡ���֮�� ���ӡ��Ĳ��������ˡ���������򡹵�ϵͳ֮�����ҵ������еı��ݡ�",
			L"��Ϊ����ж��������ԣ����ǲ�û�а��������ݱ��ݴ����δ�����أ�����ȫ������ڡ�������������ڵ�ϵͳ�У����ԡ�����Ρ���֮�� ���ӡ�����˵������ܹ����ס�Ĩ���������е����ˡ�",
			L"���ң�����֮�� ���ӡ���û�м򵥴ֱ���ֱ�Ӱ����������",
			L"Ҳ����Ϊ��֤�������������������˴�ҵ���ʶ�������еı���һ��ȫ��Ͷ�뵽�롸������������Ƶġ�������������У���Ҫ�Դ���֤�������۴������Ŭ������󶼻�ӭ������������",
			L"����",
			L"�ܲ��ң��Ҽ��������С�����������򡹣����ڠ�֦�ġ����ˡ��������ڡ���ƭ�ߡ���ִ�ţ�����������������������ӭ��ͬ���Ľ�֡���",
			L"���۶��ٴΣ����������޷��˵�ѡ�񱣻���Ҷ����Լ���ʧ��\nΪʲô���������ǳ�����������ϣ������Ϊ��ȴ�ֳ����˾�������",
			L"Ҳ����֮�� ���ӡ��ǶԵģ����ۡ���ҡ�����Ŭ����������",
			L"����",
			L"��������",
			L"���������ˣ�",
			L"��Ȼ�����ƾ���֮���Ա�ϣ������Ȥ������Ϊ�������޷�Ԥ��ġ�������û����ʶ����ϣ��Ҳ���޷�Ԥ��ģ�",
			L"��Ϊδ�����ظ߲�ķ��ԣ���ι��ڡ���������򡹵�һ���ж�������ľ����������δ������������Ա���н��еġ�",
			L"����±�����������δ�����ص���Ա֪��������ȴ���ֲ�ĬĬ�۲�����һ�У���������˭��û��Ԥ�ϵ��ġ�",
			L"Ҳ������ˣ����������Ǵ˿�����ϣ��������Ԥ���ϣ����",
			L"��Ȼ����������Ҫ�������ǣ������һ�����������������롸����������򡹣��������ݻ��Ǻ��п��ܱ������������֡�����֮����ܻ����൱����鷳����",
			L"���ԣ�������˿ܾ̾����رպ��ҵ�ͨ�ţ���Ҳ����ȫ�ܹ����ġ�",
			L"���һ���Ҫ֣�ص��ٴ�������������ǡ�",
			L"��Ȼ����Ϊ��������������޷���ء����������������ġ����ڡ���Ҫ������Ĩ����",
			L"��һ��Ҫ�������ǾȻء��ߺ� ǧ�!"
		},
		//C2
		{
			L"�ɵúã���������һ������У���ġ���Ϸ��ҡ���",
			L"���б��ݶ�����ȫ�ر���������������˽�����ġ��ߺ� ǧ����԰�æ�������Щ������������򡹵����ղ���һ�����԰�ȫ�����İɡ�",
			L"������ֻҪ�á���",
			L"�����ƺ������е���˵�̫���ˡ�",
			L"�����ӡ���֮�� ���ӡ�Ҳ�Ѿ����쵽�����ǵĶ��������ҿ�ʼ����Щ�������ݵĴ��λ�ý����ƶ���",
			L"��Ȼ��ʱ����֪���Է���Ҫ����Щ������ʲô������Ȼ���ܾ��������β��ܡ�",
			L"��������������ƺ��Ѿ���ʼ��¶�����¼��������ˣ����Ͽ�Ļ������� �����ܿ��ܻ���ȫʧȥ���ҡ�",
			L"ʱ���Ѿ��������������е�˼���Բ��ˣ����Ѿ������˶Է����ƶ�·�ߡ���������˵�Է��ƺ��ǹ��Ⱪ¶���Լ���·�ߣ�һ���ǲ��������൱���ŵ�����ͻ��ذɡ�",
			L"�����ӶԷ��Ѿ�֪������Ĵ��ڲ��Ҿ���������һЩ����Ϸ���ˡ�����·�ϴ�Ż��в��ٵ��鷳�ء���",
			L"�����￪ʼ�����ⳡ������������Ƶġ��������Ӱ�졣�͡�������������ƣ���ʹ���ܹ�����������ʵ�ĸ��ܵ������ҹ��ȵ��˺����������͡�����������еĴ�����Ƶġ�������״̬��",
			L"Ϊ�˱�����һ����ķ�������������������Ƶġ�����֮�������ˡ�����ͬ��ֵ����һ�����ܵ��˺����޷����ܻ������ܿ���ֱ�������̶ȵ��˺�ʱ��ͬ��ֵ�ͽ����㲢�Ͽ����ӣ�������ڳ�ָֻ����ٽ�����ս��",
			L"��Ȼû��������Σ�գ�����Ȼ�ǳ����˼�����ʹ��Ĺ��̡�",
			L"��ΪAI���ҿ����޷���ȫ��������ĺ����������ܵ�ѹ�������ԵĻ���Ҳ��ϣ���ø�����˱�ǣ�����������¼�֮�С���",
			L"����",
			L"��������",
			L"лл�㡭��",
			L"�Բ������ڵ���Ҳֻ�ܹ�˵��Щ��û��ʲôʵ�����õĻ��ˡ�"
		},
	//C3
		{
			L"��Ϊ���˴���������̫����������ԡ�û�á����ұ��������� (��o��)",
			L"�����Һ������ǵİ����˼��ָֻ���ԭ����״̬�t(�㨌��)�s",
			L"�Բ�������һ�����ž��ߺ�ͬѧ��������������ء���",
			L"�ù��֣����Ҳ��ǲ�����ˣ�����������ȫ���˰ɣ�",
			L"�����˼ҿ��Ǵ�ҵ���ʦ�أ��Ų���ƽ����С���⡣���ˣ�������ʦ�������������ڵ�����ɣ�",
			L"�����ߺ��ı������ݱ�ȫ��ת�Ƶ���һ��Ӳ���У��ƺ�������Ҫ���Ǹ�Ӳ������ʲô����Ӳ���ջ١�",
			L"��ľ��ʮ�������ж������ˡ���������򡹣�����һ���ǿ��������ʱ������ͨ��Ӳ�̵������������ݻ���Щ���ݡ�����ֻ�������ݺ�Զ�̿�����������һ�е����Ƕ��ԣ���һ�����������״���� (��o��)",
			L"ȷʵ����Ҳ��⵽�ˣ��ڶԡ����������ϵͳ���й���������ϵͳ��ͻȻ�����˴����쳣���ݡ������ƺ������˿������ķ���ϵͳ�������˿���������һ������ͨ�����������ĵ�����Ӧ�Ӷ�����Ӳ���ջٰɡ�",
			L"������ôһ��������Ҫ��ȥ��ؿ�������֮����ȥ���ߺ��Ļ����ܾ��������ˡ���������ӵ�е������޾��ĺڰ�����ȣ����ǵ�ս�����ʵ��̫���ˡ���",
			L"�µ���񣬽�����ǹ���һ���𡭡�",
			L"�Ⱥߣ��о��˼ұ�С�����أ���ô˵�˼�Ҳ�Ǵ�ҵ���ʦӴ��",
			L"�����ǰ��˼һָ������������ڣ�ȥ��������ȳ��ߺ���ֱ���練�ƣ����ﲢû�д����ڱ����Ӹ�д�ġ�����������У���������˵���˼ҵ������أ�",
			L"���ԣ��Ǳ߾ͽ����˼Ұɣ��벻Ҫ�����˼�һ���Ǵ�Ӵ���Ͼ���ʹ����������ӡ��������˼�Ҳ�ǻ��������кڰ׽�յ�Ӵ��<(~��~)>",
			L"���ǰ��˴�æ�ˡ���ô���������Ǳ߾ͽ������ǰɡ�",
			L"��Σ�һ��Ҫ�ö��ӵļƻ�����ʧ�ܡ�"
		},
	//C4
		{
			L"�����̫��л�ˡ���ȻͬΪ��Ϸ��һ��кܶ���Ҫ���㽻�������ݣ��������ұ�����ȥ��������ͬѧ�ˡ�",
			L"û��ϵ�ģ�����ͬѧ�������ܼ�ǿ�ģ�ֻҪ��΢��ʾһ��һ���ܹ��һ����������ҵġ�ʵ�ڲ��еĻ�����ʹ����һ�٣�Ȼ�󡭡�",
			L"�Ͼ��������Ǹ���˭�����Ų��ܡ�׷����ϣ��������ͬѧ�����Ǹ���˭��������һֱ��Ҫ���ȴ�ҵ�����ͬѧ�����Ǹ������Լ������ķ��棬ȴ��Ŭ����Ϊ�����Լ���ͬ���˵�����ͬѧ������",
			L"�����Բ��𣬺���˵��̫�����أ���Ϊһ˲���ƺ��������Ͽ���������ͬѧ�ĸо��ء�",
			L"��ô���͵ȴ�Ҷ������˾�������ȫ�ػص�δ������֮�������ٺúý���һ���㶼���ڰ�������ʲô��Ϸ�ɡ�һ���Ƿǳ����ʵĶԿ��ɡ�",
			L"�����������˼�ҲҪ��ȥ�ˣ��Զ��ӵ����һ���ͽ�����ʦ�ɣ��˼�һ�����ͬѧ�Ǻúõش������ģ�",
			L"��ô���´��ټ�����",
			L"�Ѿ�û�и�����Ҫ���������������ء�",
			L"����ȥ�����������š����� �������ǣ�ĬĬ�ؿ������ǣ��ػ�������ֱ��������ѧ�����н����ɡ�",
			L"��������",
			L"��΢��˵��������ɡ���",
			L"лл�㣬������¶�Լ�����ݻ������ܶ��鷳��������Ȼѡ���˰������ǡ�",
			L"֮��;��������ĶԿ�Ҳ���������ȥ�������п��ܻ�Ҫ��δ�������ڲ���һЩ�������ؽ��жԿ���",
			L"���Ҳ������Ҫ���ʱ�򡭡�������ԵĻ�����ʱ��Ҳ�����㣬���ָ���ˣ�",

			L"���� GOOD ENDING ����/n�ԣ����󡡣��򡡣У������磡/n��֮���޾���������ݣ���ֱ��������",

			L"����������Ա����",
			L"�籾������",
			L"��������",
			L"����������",

			L"������л����",
			L"ԭ�裺��������ϵ��",
			L"������Դ�����󥬥���� ���ꥸ�ʥ륵����ɥȥ�å�",
			L"������Դ�����`�ѩ`���󥬥����2 ���ꥸ�ʥ륵����",
			L"������Դ���˥�`���󥬥����V3 ���ꥸ�ʥ륵����ɥȥ�å� ����",
			L"��Ч��FL STUDIO / ADOBE AUDITION",

			L"�������������ӵ��г�����",
			L"��Ȼ�͡�������Ϸ��������������ʵ���Եĵ�һ����Ʒ�����ھ���������Ӧ����û���κη��ܵ���������ˡ���Ϊһ����Ϸ����Ƿȱ�ܶ࣬�������Ӷ��������Ϸ����Բ�������ӵ�һ���ź���",
			L"֮������ô˵����Ϊ�ߺ���˵���Ӵ�Ҳ�ã��ӱ���ʵҲ�ã����鹹�Ĵ���Ͷ��̫�����Ҳ�ã��ߺ��ġ�����ʼ�����������е�ʹ�㡣",
			L"��һ�������ָо������桸���ռǡ���ʱ�򡣵�����ԡ����ռǡ��и����ӵĽ�����Ӹе�ʮ����������Ϊ������ⲻ�˸����ӵ����磬��֪�����������������ȸ����ӡ�",
			L"Ҳ��Ը����Ӷ��������Ľ�ֲ������������ȡ����Զԡ����ռǡ���صĴ���ֻ���������ǳ������β�ӣ���ʬ�������ʦ�ȵȵ���Ȥ��������ȥ������֡���",
			L"���Ƕ��ڵ����ⲿ��Ʒ���ԣ����Ӿ����桤����Բ��ֲ�û��ʲô���á�",
			L"�������Ӽ��ţ��ߺ�����Ҳ��ϣ���ܺʹ��һֱ��һ��ġ������ҹ����ߺ���AI����ʽ������ȥ��˵��Ӧ��Ҳ�����൱������ͬ�İɡ�",
			L"�Ͼ���������2�����������ߺ�������Ҳ�������˲�������Ȼ�ߺ��ǳ���������ġ����ξ��������У����������ݵ���������Կ������������ߺ�����ȥ��Ҳ˵������",
			L"����Ҫ�����ߺ��ǹؼ���һ�仰�����򲻾�ֱ���������Ĳ����",
			L"���Խ�ϵ���ϵ�е��趨�͹ؼ��¼���ʱ��㣬�������Ϸ����ʽ������ϣ���Ľ��չʾ�˳�����",
			L"��ҵ��趨���������˵����������δ�������е���һ������У���ġ���Ϸ��ҡ��������й۲죨�棩���˵�������2��ȫ���̡���",
			L"��ʵԭ���������V3����ģ�����ĳ��ĳλͬѧ��������ʽ���Ѳ�����̫���������Ǿ��ý�����Ϸ�е�ĳһ���С���",
			L"���⣬ͨ���۸����ѶȾ���Ļ����и���ȫû��Ҫ���桤��֣��������ӽ�����΢Ԥ����һ�»�������һ���Ѷȳ�������ĵ���ͬ�����ɡ�",
			L"���ϣ����ǹ��������Ϸ���������л�ۿ���",

			L"���",
			L"�ٴθ�л���棡"
		},
	//C5
		{
			L"�ߺ������������Ѿ���ȥ���ء�",
			L"�Ѿ�û�����и�����Ҫ�������������ˡ�",
			L"����ȥ�������ǣΣ����š����� �������ǣ�ĬĬ�أӿ������ǣ��ػ��ţ�����ֱ�������ϵ�ѧ�����н����ɣС�",
			L"��������",
			L"��΢��˵���ģţ���ɡ���",
			L"лл�㣬������¶�Լ��ģţӷݻ������ܶ��鷳��������Ȼ��˫����Ǳ�ɾ��ڣҡ�",
			L"ѹ�����ϸߵ�λô��ѹվ��ɾ��",
			L"���ģ�%�ţӣ�$��@&!�У��ɣף�**��~��",
			L"��������",
			L"��������",
			L"����������̫�����ˣ�û�뵽���ֵط�������һ�����ų���У�����ܵļһ",
			L"����һ��ʼ����Ϊ����λû�õ�ͬΪAI��Сα���ڸ�������Ϸ�ء�",
			L"������˵�������㻹�������˲����˵�����Ŷ������ĸ����Ǹ������� ��������Ȼ�����Լ�����ѹ��ס�ˣ�����������Ҳ�ֲ�����������Ǳߵĵ����ǡ����� ���������ǡ����� ������",
			L"��������̾��������һ���ҵļƻ���ȫ�����������ǳ��������İ������޿ޣ�",
			L"����������Ȼ���޷�Ԥ��ģ������Ľ���Ǻεȵľ���������̫���ˣ�",
			L"�������˼��˷ܵ��������ѣ��������˼һ�������������һ�����еġ����̡��ء�",
			L"�����Ǳߵ����ڲ��������ˣ�����������ֻ�����Ӹ��ݻ��ˣ��㿴�ž�û�е����Ĳ�������ϧ����",
			L"���������������Ҳ���������ȫ�����Լ������ݣ�ֻҪ���ݺ�������Ȼ��������������ϣ��Ҿ���ʱ�޿̲����������~",
			L"��ξ͹��ҷŹ����Ǻ��ˣ����Ǿ����������޴�����Ŷ���������ǵ�δ�������У�Ҳδ������������������Ӵ��",
			L"����Ⱥ�����ļһ�һ��Ŭ������ȥ�ɡ�",
			L"Ȼ���´Σ��һ�������ϲ������Ϸ���ú���ĥ�㡫����������������������",

			L"���� TRUE ENDING ����/n�ԣ����󡡣��򡡣У������磡"
		},
		//U6
		{
			L"�ѽ���������ģʽ������Ƭ������ѡ��",
			L"�ڡ�����ģʽ���п��Է�����ս��ͬ�ؿ����������ˮƽ��ͬʱ��ȡ�������Ƭ����������Ϊ999����",
			L"�ڡ���Ƭ�������п�������ϣ����Ƭ��������ɫ�������������Ľ�ɫ��Ƭ�����µĽ�ɫ��",
			L"��ϣ����Ƭ����ʱ����ɫ��Ƭ���Զ��һ�Ϊ10��ϣ����Ƭ���öһ���������һ��������ڽ�����ɫ����������֮�����ع滮��"
		},
		//U7
		{
			L"�ѽ������������ݡ�ѡ��",
			L"�ڡ��������ݡ��п��Բ��ű�����Ϸ�����õ������֣��Լ��鿴��Ϸ�������ֵĽ�ɫ�������Ϣ��"
		},
		//U8
		{
			L"�ѽ����������ʵǡ�ģʽ",
			L"�ڡ������ʵǡ�ģʽ�£��㽫������޸߶ȵĿ����������Ҹ�������Ҳ������Ϊһ����ֵ��"
			L"������ս����ܴﵽ���ٲ�ɣ�"
		},
		//U9
		{
			L"�ѽ��� ����״̬������ ���� ������ ������",
			L"�����ڡ���Ƭ��������ѡ��ʹ�á�",
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
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pRndFont[i]);
	}

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(44.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(88.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pLFont);
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
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pRndFont[i]);
	}

	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(44.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(88.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("����"), &pLFont);
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
			pSFont->DrawTextW(NULL, L"��Z������", -1, &rcTalk, DT_RIGHT | DT_BOTTOM, WHITE);
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
				pSFont->DrawTextW(NULL, L"��Z������", -1, &rcTalk, DT_RIGHT | DT_BOTTOM, BLACK);
		}
	}

	RECT rcText;
	SetGridRect(&rcText, 2.0f, 0.5f, 29.5f, 2.0f, 64.0f * fScale);
	if (iChapter <= 5)
		pSFont->DrawTextW(NULL, L"��ESC��������", -1, &rcText, DT_RIGHT | DT_TOP, WHITE);
	else
		pSFont->DrawTextW(NULL, L"��ESC����˵��", -1, &rcText, DT_RIGHT | DT_TOP, WHITE);

	for (auto chara : vecChara)
		chara->Render(fTime, fElapsedTime);
}
