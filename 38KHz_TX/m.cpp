#include <iostream>
#include <vector>

using namespace std;

// �����ƽ
vector<int> startLevel = { 9000, 4500 };  // 起始码
vector<int> linkLevel = { 550, 20000 };   // 连接码
vector<int> lowLevel = { 550, 550 };      // 低电平
vector<int> highLevel = { 550, 1660 };    // 高电平

// 定义电平
int modeFlag = 4;
vector<int> modeCodeFunc(int m) {
    vector<vector<int>> modeCode = {
        {lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]},  // 自动
        {highLevel[0], lowLevel[1], lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]}, // 制冷
        {lowLevel[0], highLevel[1], lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]}, // 加湿度
        {highLevel[0], highLevel[1], lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]},// 送风
        {lowLevel[0], lowLevel[1], highLevel[0], highLevel[1]}                           // 制热
    };
    if (m > modeCode.size() - 1) {
        cout << "模式参数必须小于" << modeCode.size() << endl;
        return modeCode[0];
    }
    modeFlag = m;
    return modeCode[m];
}

// 开关
int keyFlag = 0;
vector<int> keyCodeFunc(int k) {
    vector<vector<int>> keyCode = {
        {lowLevel[0], lowLevel[1]},   // 关
        {highLevel[0], highLevel[1]}  // 开
    };
    keyFlag = k;
    return keyCode[k];
}

// 风速
int fanSpeedFlag = 0;
vector<int> fanSpeedCodeFunc(int f) {
    vector<vector<int>> fanSpeedCode = {
        {lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]},  // 自动
        {highLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]}, // 一档
        {lowLevel[0], highLevel[1], lowLevel[0], lowLevel[1]}, // 二档
        {highLevel[0], highLevel[1], lowLevel[0], lowLevel[1]} // 三档
    };
    if (f > fanSpeedCode.size() - 1) {
        cout << "风速参数必须小于" << fanSpeedCode.size() << endl;
        return fanSpeedCode[0];
    }
    fanSpeedFlag = f;
    return fanSpeedCode[f];
}

// 扫风
vector<int> fanScanCodeFunc(int f) {
    vector<vector<int>> fanScanCode = {
        {lowLevel[0], lowLevel[1]},
        {highLevel[0], highLevel[1]}
    };
    if (f > fanScanCode.size() - 1) {
        cout << "扫风参数必须小于" << fanScanCode.size() << endl;
        return fanScanCode[0];
    }
    return fanScanCode[f];
}

vector<int> getSleepCode(int s) {
    vector<vector<int>> sleepCode = {
        {lowLevel[0], lowLevel[1]},
        {highLevel[0], highLevel[1]}
    };
    if (s > sleepCode.size() - 1) {
        cout << "睡眠参数必须小于" << sleepCode.size() << endl;
        return sleepCode[0];
    }
    return sleepCode[s];
}

int tempFlag = 16;
vector<int> tempertureCodeFunc(int t) {
    tempFlag = t;
    vector<int> tempCode;
    int dat = t - 16;
    for (int i = 0; i < 4; i++) {
        int x = dat & 1;
        if (x == 1) {
            tempCode.insert(tempCode.end(), highLevel.begin(), highLevel.end());
        }
        else if (x == 0) {
            tempCode.insert(tempCode.end(), lowLevel.begin(), lowLevel.end());
        }
        dat = dat >> 1;
    }
    return tempCode;
}

// 定时参数
vector<int> getTimerCode() {
    vector<int> timerCode(8, lowLevel[0]);
    return timerCode;
}

// 超强、灯光、健康、干燥、换气
vector<int> getOtherCode(bool strong, bool light, bool health, bool dry, bool breath) {
    vector<int> otherFuncCode;

    // 依次添加 strong, light, health, dry, breath 的高电平或低电平
    // 函数用于决定使用 highLevel 还是 lowLevel
    auto chooseLevel = [](bool condition) {
        return condition ? highLevel : lowLevel;
        };

    // 添加每种功能的代码
    for (auto level : { chooseLevel(strong), chooseLevel(light), chooseLevel(health),
                        chooseLevel(dry), chooseLevel(breath) }) {
        otherFuncCode.insert(otherFuncCode.end(), level.begin(), level.end());
    }

    return otherFuncCode;
}

// 前35位结束码后七位结束码
vector<int> getFirstCodeEnd() {
    return { lowLevel[0], lowLevel[1], lowLevel[0], highLevel[1], lowLevel[0], highLevel[1], lowLevel[0] };
}

// 连接码
vector<int> getLinkCode() {
    vector<int> linkCode = lowLevel;
    linkCode.insert(linkCode.end(), highLevel.begin(), highLevel.end());
    linkCode.insert(linkCode.end(), lowLevel.begin(), lowLevel.end());
    linkCode.insert(linkCode.end(), linkLevel.begin(), linkLevel.end());
    return linkCode;
}

// 上下扫风
int fanUpAndDownFlag = 1;
int fanLeftAndRightFlag = 1;
vector<int> fanUpAndDownCodeFunc(int f) {
    vector<vector<int>> fanUpAndDownCode = {
        {lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]},
        {highLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]}
    };
    fanUpAndDownFlag = f;
    fanScanCodeFunc(fanUpAndDownFlag || fanLeftAndRightFlag);
    return fanUpAndDownCode[f];
}

// 左右扫风
vector<int> fanLeftAndRightCodeFunc(int f) {
    vector<vector<int>> fanLeftAndRightCode = {
        {lowLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]},
        {highLevel[0], lowLevel[1], lowLevel[0], lowLevel[1]}
    };
    fanLeftAndRightFlag = f;
    fanScanCodeFunc(fanUpAndDownFlag || fanLeftAndRightFlag);
    return fanLeftAndRightCode[f];
}

// 固定码
vector<int> getOtherFunc2() {
    vector<int> otherFunc2;

    // ���12�� lowLevel[0]
    otherFunc2.insert(otherFunc2.end(), 12, lowLevel[0]);

    // ������� { lowLevel[0], highLevel[1], lowLevel[0], lowLevel[1] }
    otherFunc2.insert(otherFunc2.end(), { lowLevel[0], highLevel[1], lowLevel[0], lowLevel[1] });

    // ���12�� lowLevel[0]
    otherFunc2.insert(otherFunc2.end(), 12, lowLevel[0]);

    return otherFunc2;
}

vector<int> getCheckoutCode() {
    // 校验码 = (模式 – 1) + (温度 – 16) + 5 + 左右扫风 + 换气 + 节能 - 开关
    // 取二进制后四位，再逆序
    int dat = (modeFlag - 1) + (tempFlag - 16) + 5 + fanLeftAndRightFlag + 0 + 0 - keyFlag;
    vector<int> code;
    for (int i = 0; i < 4; i++) {
        int x = dat & 1;
        if (x == 1) {
            code.insert(code.end(), highLevel.begin(), highLevel.end());
        }
        else if (x == 0) {
            code.insert(code.end(), lowLevel.begin(), lowLevel.end());
        }
        dat = dat >> 1;
    }
    return code;
}

vector<int> getSecondCodeEnd() {
    return { 550, 40000 };
}

int main() {
    cout << "格力空调遥控器红外编码-长码" << endl;
    cout << "100032-格力9" << endl;
    vector<int> code = { 9000, 4500 };
    
    //code.insert(code.end(), startLevel.begin(), startLevel.end()); // 起始码

    vector<int> modeCode = modeCodeFunc(1); // 模式：0自动，1制冷，2加湿，3送风，4加热
    
    for (int val : modeCode) {
        code.push_back(val);
    }

    vector<int> keyCode = keyCodeFunc(1); // 开关：0关，1开
    for (int val : keyCode) {
        code.push_back(val);
    }

    vector<int> fanSpeedCode = fanSpeedCodeFunc(0); // 风速：0自动，1一档，2二档，3三档
    for (int val : fanSpeedCode) {
        code.push_back(val);
    }

    vector<int> fanScanCode = fanScanCodeFunc(0); // 扫风：0关，1开
    for (int val : fanScanCode) {
        code.push_back(val);
    }

    vector<int> sleepCode = getSleepCode(0); // 睡眠
    for (int val : sleepCode) {
        code.push_back(val);
    }

    vector<int> tempCode = tempertureCodeFunc(16); // 温度
    for (int val : tempCode) {
        code.push_back(val);
    }

    vector<int> timerCode = getTimerCode(); // 定时
    for (int val : timerCode) {
        code.push_back(val);
    }
    printf("good");
    vector<int> otherCode = getOtherCode(false, true, false, false, false); // 其他-超强、灯光、健康、干燥、换气
    for (int val : otherCode) {
        code.push_back(val);
    }
    
    vector<int> firstCodeEnd = getFirstCodeEnd(); // 剩余的编码
    for (int val : firstCodeEnd) {
        code.push_back(val);
    }
    
    vector<int> linkCode = getLinkCode(); // 连接码
    for (int val : linkCode) {
        code.push_back(val);
    }
    
    vector<int> fanUpAndDownCode = fanUpAndDownCodeFunc(0); // 上下扫风
    for (int val : fanUpAndDownCode) {
        code.push_back(val);
    }
    
    vector<int> fanLeftAndRightCode = fanLeftAndRightCodeFunc(1); // 左右扫风
    for (int val : fanLeftAndRightCode) {
        code.push_back(val);
    }
    
    vector<int> otherFunc2 = getOtherFunc2(); // 固定码
    for (int val : otherFunc2) {
        code.push_back(val);
    }
    
    vector<int> checkoutCode = getCheckoutCode(); // 校验码
    for (int val : checkoutCode) {
        code.push_back(val);
    }

    vector<int> secondCodeEnd = getSecondCodeEnd(); // 结束码
    for (int val : secondCodeEnd) {
        code.push_back(val);
    }

    cout << "电平码" << endl;
    for (auto c : code) {
        cout << c << " ";
    }
    cout << endl;
    cout<<"go"<<endl;

    return 0;
}

