#include <bangtal>
using namespace bangtal;

//사각형 클래스 생성
class Rect {
	int centerX, centerY;
	int radius;

public:
	Rect(int cx, int cy, int r) : centerX(cx), centerY(cy), radius(r) {};
	bool checkIn(int x, int y) const {
		return (x > centerX - radius && x < centerX + radius && y > centerY - radius && y < centerY + radius);
	}
};

ScenePtr scene = nullptr;
const auto check_radius = 25;

//틀린점 클래스 생성
class DifferencePoint {
	Rect left_rect, right_rect;
	ObjectPtr left_check, right_check;
	bool shown = false;

public:
	DifferencePoint(int lcx, int rcx, int cy, int r) : left_rect(lcx, cy, r), right_rect(rcx, cy, r) {
		left_check = Object::create("images/check.png", scene, lcx - check_radius, cy - check_radius, false);
		right_check = Object::create("images/check.png", scene, rcx - check_radius, cy - check_radius, false);
	}
	bool checkIn(int x, int y) const {
		return left_rect.checkIn(x, y) || right_rect.checkIn(x, y);
	}
	void show() {
		left_check->show();
		right_check->show();

		shown = true;
	}
	bool found() const { return shown; }
};

//목숨 클래스 설정
class Life {
	int X, Y;
	ObjectPtr heart;

public:
	Life(int x, int y) : X(x), Y(y) {
		heart = Object::create("images/heart.png", scene, X, Y);
		heart->setScale(0.03f);
	}
	void hide() {
		heart->hide();
	}
};

int main()
{
	//필요없는 옵션 제거
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	scene = Scene::create("틀린그림찾기", "images/problem.png");

	auto problem = Object::create("images/problem.png", scene, 0, 0);
	const auto num_of_differences = 7;

	//틀린점 처리
	DifferencePoint points[num_of_differences] = {
		{ 538, 1156, 574, 54 },
		{ 99, 717, 551, 17 },
		{ 383, 1001, 482, 16 },
		{ 401, 1019, 162, 27 },
		{ 61, 679, 265, 36 },
		{ 592, 1210, 451, 35 },
		{ 320, 938, 122, 13 },
	};

	auto count = 0;

	//3개의 목숨 생성
	int life_score = 3;
	Life life[3] = {
		{595,630},
		{625,630},
		{655,630},
	};
	problem->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		auto wrong = true;
		for (auto i = 0; i < num_of_differences; ++i) {
			if (points[i].checkIn(x, y)) {
				if (!points[i].found()) {
					points[i].show();
					count++;
				}
				wrong = false;
			}
		}

		if (wrong) {
			life_score--;
			showMessage("땡!!");
			life[life_score].hide();
			if (life_score == 0)
			{
				showMessage("실패했어요... 1초뒤에 종료됩니다.");
				auto timer2 = Timer::create(1);
				timer2->start();

				timer2->setOnTimerCallback([&](TimerPtr timer)->bool {
					endGame();
					return true;
					});
			}
		}

		if (count == num_of_differences) {
			showMessage("성공!!!");

			auto timer2 = Timer::create(1);
			timer2->start();

			timer2->setOnTimerCallback([&](TimerPtr timer)->bool {
				endGame();
				return true;
				});
		}
		return true;
		});

	//60초 제한 설정
	auto timer1 = Timer::create(60);
	showTimer(timer1);
	timer1->start();

	timer1->setOnTimerCallback([&](TimerPtr timer)->bool {
		showMessage("실패ㅠㅠ");
		auto timer2 = Timer::create(1);
		timer2->start();

		timer2->setOnTimerCallback([&](TimerPtr timer)->bool {
			endGame();
			return true;
			});
		return true;
		});
	showMessage("기회는 3번!! 틀린그림을 찾아보세요!");
	startGame(scene);

	return 0;
}