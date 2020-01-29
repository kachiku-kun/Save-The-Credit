
# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene(init)
{
	player = Texture(U"img/eto_saru_dance.png", TextureDesc::Mipped);
	credit =  Texture(U"img/school_text_tani.png", TextureDesc::Mipped);
	ramen =  Texture(U"img/ramen_moyashi.png", TextureDesc::Mipped);
	beer = Texture(U"img/drink_beer.png", TextureDesc::Mipped);
	thesis = Texture(U"img/document_sotsugyou_ronbun_taba.png", TextureDesc::Mipped);

	heart = Texture(Emoji(U"💗"));
	skull = Texture(Emoji(U"💀"));
}

void Game::object_init() {
	int32 kind = 0;
	if (RandomBool(0.06)) {
		kind = 1;
	}

	if (RandomBool(0.06)) {
		kind = 3;
	}

	if (RandomBool(0.12)) {
		kind = 2;
	}

	item << Item(Rect(Random(600), 0, size), kind);
}

void Game::update()
{
	// 一定時間経過したらバフデバフは解除
	if (effectTime.sF() > 12.0) {
		isDebuff = 0;
		isBoost = 0;

		effectTime.pause();
	}

	// 一定周期でオブジェクトを生成する
	if (m_score > 100) {
		if (flame % obj_time_fast == 0) {
			object_init();
		}
	}
	else if (m_score > 50) {
		if (flame % obj_time_midium == 0) {
			object_init();
		}
	}
	else {
		if (flame % obj_time_slow == 0) {
			object_init();
		}
	}

	// プレイヤを操作
	// AかLeftが押されたら左移動
	if (KeyA.pressed() || KeyLeft.pressed()) {
		if (isBoost) {
			m_player.x -= boost;
		}
		else if (isDebuff) {
			m_player.x -= debuff;
		}
		else {
			m_player.x -= player_speed;
		}
	}
	// DかRightが押されたら右移動
	if (KeyD.pressed() || KeyRight.pressed()) {
		if (isBoost) {
			m_player.x += boost;
		}
		else if (isDebuff) {
			m_player.x += debuff;
		}
		else {
			m_player.x += player_speed;
		}
	}

	// オブジェクトを移動
	for (auto& item : item) {
		item.move();
	}

	// 画面から消えたオブジェクトを消去
	for (auto it = item.begin(); it != item.end(); ++it) {
		if (it->rect.y > 600) {
			// 取り逃したときの効果
			if (it->kind == 0) {
				life--;
			}
			if (it->kind == 3) {
				life = 0;
			}

			item.erase(it);

			break;
		}
	}

	// プレイヤと接触したオブジェクトを消去
	for (auto it = item.begin(); it != item.end(); ++it) {
		if (it->rect.intersects(m_player)) {
			// 取ったときの効果
			// 単位を取ったら2点
			if (it->kind == 0) {
				m_score += 2;
			}
			// 二郎を取ったらブースト，すでにブーストならデバフ
			else if (it->kind == 1) {
				if (isBoost) {
					isBoost = 0;
					isDebuff = 1;

					effectTime.restart();
				}
				else {
					isBoost = 1;
					isDebuff = 0;

					effectTime.restart();
				}
			}
			// 酒を取ったらデバフ
			else if (it->kind == 2) {
				isBoost = 0;
				isDebuff = 1;

				effectTime.restart();
			}
			// 卒論を取ったら4点
			else {
				m_score += 4;
			}

			item.erase(it);
			break;
		}
	}

	// ライフが0になったら終了
	if (life <= 0) {
		getData().highScore = Max(getData().highScore, m_score);

		changeScene(State::Title);
	}

	flame++;
}

void Game::draw() const
{

	FontAsset(U"Score")(U"SCORE").drawAt(625, 30);
	FontAsset(U"Score")(m_score).drawAt(725, 30);

	FontAsset(U"Score")(U"LIFE").drawAt(50, 30);
	if (life >= 1) {
		heart.resized(50,50).drawAt(120,30);
	}else{
		skull.resized(50, 50).drawAt(120, 30);
	}

	if (life >= 2) {
		heart.resized(50, 50).drawAt(170, 30);
	}
	else {
		skull.resized(50, 50).drawAt(170, 30);
	}

	if (life == 3) {
		heart.resized(50, 50).drawAt(220, 30);
	}
	else {
		skull.resized(50, 50).drawAt(220, 30);
	}

	if (isBoost) {
		FontAsset(U"Score")(U"！優勝！").drawAt(Scene::Center().x, 30, Palette::Yellow);
		FontAsset(U"Score")(12 - effectTime.s()).drawAt(Scene::Center().x, 60);
	}
	else if (isDebuff) {
		FontAsset(U"Score")(U"！二日酔い！").drawAt(Scene::Center().x, 30, Palette::Red);
		FontAsset(U"Score")(12 - effectTime.s()).drawAt(Scene::Center().x, 60);
	}


	// すべてのオブジェクトを描画する
	for (const auto& item : item) {
		if (item.kind == 0) {
			item.rect(credit).draw();
		}
		else if (item.kind == 1) {
			item.rect(ramen).draw();
		}
		else if (item.kind == 2) {
			item.rect(beer).draw();
		}
		else {
			item.rect(thesis).draw();
		}
	}

	// プレイヤを描く
	m_player(player).draw();
}
