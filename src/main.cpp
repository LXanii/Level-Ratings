#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(LevelCell) {
	
CCSprite* ratingSprite;
CCLabelBMFont* ratingLabel;

	void loadCustomLevelCell() {
		LevelCell::loadCustomLevelCell();

		CCLayer* mainLayer = static_cast<CCLayer*>(getChildByID("main-layer"));
		CCSprite* likeSprite = static_cast<CCSprite*>(mainLayer->getChildByID("likes-icon"));
		CCLabelBMFont* likeLabel = static_cast<CCLabelBMFont*>(mainLayer->getChildByID("likes-label"));
		CCNode* likeLabelEnding = reinterpret_cast<CCNode*>(likeLabel->getChildren()->objectAtIndex(likeLabel->getChildrenCount() - 1));

		double ratingRatio = floor(((double(m_level->m_likes)/double(m_level->m_downloads))*100)*100+0.5)/100;
		
		m_fields->ratingLabel = CCLabelBMFont::create(fmt::format("{}%", ratingRatio).c_str(), "bigFont.fnt");
		m_fields->ratingSprite = CCSprite::createWithSpriteFrameName("diffIcon_01_btn_001.png");

		if (ratingRatio >= 8) {
			m_fields->ratingSprite = CCSprite::createWithSpriteFrameName("diffIcon_01_btn_001.png");
			m_fields->ratingLabel->setColor({ 43, 255, 0 });
		}
		else if (ratingRatio >= 6) {
			m_fields->ratingSprite = CCSprite::createWithSpriteFrameName("diffIcon_02_btn_001.png");
			m_fields->ratingLabel->setColor({ 136, 255, 0 });
		}
		else if (ratingRatio >= 4) {
			m_fields->ratingSprite = CCSprite::createWithSpriteFrameName("diffIcon_03_btn_001.png");
			m_fields->ratingLabel->setColor({ 200, 255, 0 });
		}
		else if (ratingRatio >= 2) {
			m_fields->ratingSprite = CCSprite::createWithSpriteFrameName("diffIcon_04_btn_001.png");
			m_fields->ratingLabel->setColor({ 255, 196, 0 });
		}
		else if (ratingRatio > 0) {
			m_fields->ratingSprite = CCSprite::createWithSpriteFrameName("diffIcon_05_btn_001.png");
			m_fields->ratingLabel->setColor({ 255, 123, 0 });
		}
		else {
			m_fields->ratingSprite = CCSprite::createWithSpriteFrameName("diffIcon_06_btn_001.png");
			m_fields->ratingLabel->setColor({ 255, 0, 0 });
		}
		
		m_fields->ratingSprite->setScale(0.5);
		m_fields->ratingSprite->setID("Level-Ratings/ratings-icon");

		m_fields->ratingLabel->setScale(0.4);
		m_fields->ratingLabel->setID("Level-Ratings/ratings-label");

		if (static_cast<CCSprite*>(mainLayer->getChildByID("orbs-icon"))) { // for rated levels
			CCSprite* manaLabel = static_cast<CCSprite*>(mainLayer->getChildByID("orbs-label"));
			CCNode* manaLabelEnding = static_cast<CCNode*>(manaLabel->getChildren()->objectAtIndex(manaLabel->getChildrenCount() - 1));
			m_fields->ratingSprite->setPosition(manaLabelEnding->convertToWorldSpace(getPosition()));
			m_fields->ratingLabel->setPosition(manaLabelEnding->convertToWorldSpace(getPosition()));
			m_fields->ratingSprite->setPosition({m_fields->ratingSprite->getPositionX() + 20, m_fields->ratingSprite->getPositionY() + 6});
			m_fields->ratingLabel->setPosition({m_fields->ratingLabel->getPositionX() + 52, m_fields->ratingLabel->getPositionY() + 6});
		}
		else {
			m_fields->ratingSprite->setPosition(likeLabelEnding->convertToWorldSpace(getPosition()));
			m_fields->ratingLabel->setPosition(likeLabelEnding->convertToWorldSpace(getPosition()));
			m_fields->ratingSprite->setPosition({m_fields->ratingSprite->getPositionX() + 26, m_fields->ratingSprite->getPositionY() + 6});
			m_fields->ratingLabel->setPosition({m_fields->ratingLabel->getPositionX() + 58, m_fields->ratingLabel->getPositionY() + 6});
		}

		if (likeSprite->getScale() < 0.6) { // thank you compact lists
			m_fields->ratingSprite->setScale(0.35);
			m_fields->ratingLabel->setScale(0.3);
			m_fields->ratingSprite->setPosition({m_fields->ratingSprite->getPositionX() - 7, m_fields->ratingSprite->getPositionY() - 1.5f});
			m_fields->ratingLabel->setPosition({m_fields->ratingLabel->getPositionX() - 16, m_fields->ratingLabel->getPositionY() - 1});
			if (static_cast<CCSprite*>(mainLayer->getChildByID("orbs-icon"))) {
				m_fields->ratingSprite->setPositionX(m_fields->ratingSprite->getPositionX() + 3);
				m_fields->ratingLabel->setPositionX(m_fields->ratingLabel->getPositionX() + 3);
			}
		} 
		
		mainLayer->addChild(m_fields->ratingSprite);
		mainLayer->addChild(m_fields->ratingLabel);
	}
};

class $modify(ratioDisplay, LevelInfoLayer) {

CCLabelBMFont* ratingLabel;
std::string textString;

	bool init(GJGameLevel* p0, bool p1) {
		bool result = LevelInfoLayer::init(p0, p1);
		double ratingRatio = floor(((double(m_level->m_likes)/double(m_level->m_downloads))*100)*100+0.5)/100;
		CCMenu* ratioMenu = CCMenu::create();
		CCLabelBMFont* downloadsLabel = static_cast<CCLabelBMFont*>(getChildByID("downloads-label"));

		m_fields->ratingLabel = CCLabelBMFont::create(fmt::format("({}%)", ratingRatio).c_str(), "bigFont.fnt");

		if (ratingRatio >= 8) {
			m_fields->ratingLabel->setColor({ 43, 255, 0 });
			m_fields->textString = fmt::format("Out of <cg>{} people</c> who have played this level, <cg>{}%</c> have liked it!\n\nThis indicates that a large majority of players who played this level liked it!", m_level->m_downloads, ratingRatio).c_str();
		}
		else if (ratingRatio >= 6) {
			m_fields->ratingLabel->setColor({ 136, 255, 0 });
			m_fields->textString = fmt::format("Out of <cg>{} people</c> who have played this level, <cg>{}%</c> might have enjoyed it!\n\nThis indicates that a majority of players who played this level enjoyed it!", m_level->m_downloads, ratingRatio).c_str();
		}
		else if (ratingRatio >= 4) {
			m_fields->ratingLabel->setColor({ 200, 255, 0 });
			m_fields->textString = fmt::format("Out of <cg>{} people</c> who have played this level, <cy>{}%</c> have somewhat liked it.\n\nThis indicates players who played this somewhat enjoyed the level.", m_level->m_downloads, ratingRatio).c_str();
		}
		else if (ratingRatio >= 2) {
			m_fields->ratingLabel->setColor({ 255, 196, 0 });
			m_fields->textString = fmt::format("Out of <cg>{} people</c> who have played this level, <co>{}%</c> have decided its okay.\n\nThis indicates that the level may possibly not be the best for enjoyment.", m_level->m_downloads, ratingRatio).c_str();
		}
		else if (ratingRatio > 0) {
			m_fields->ratingLabel->setColor({ 255, 123, 0 });
			m_fields->textString = fmt::format("Out of <cg>{} people</c> who have played this level, only <co>{}%</c> have gave this a like.\n\nThis indicates that this level in particular is probably not too fun to play.", m_level->m_downloads, ratingRatio).c_str();
		}
		else {
			m_fields->ratingLabel->setColor({ 255, 0, 0 });
			m_fields->textString = fmt::format("Out of <cg>{} people</c> who have played this level, <cr>{}%</c> have liked this. That's literally nobody.\n\nJust run, probably a bad level.", m_level->m_downloads, ratingRatio).c_str();
		}

		ratioMenu->setPosition({downloadsLabel->getPositionX() + 23, downloadsLabel->getPositionY() - 14});
		m_fields->ratingLabel->setScale(0.4);
		m_fields->ratingLabel->setID("Level-Ratings/ratings-label");

		CCMenuItemSpriteExtra* ratioButton = CCMenuItemSpriteExtra::create(reinterpret_cast<CCSprite*>(m_fields->ratingLabel), this, menu_selector(ratioDisplay::displayText));
		ratioMenu->addChild(ratioButton);

		this->addChild(ratioMenu);

		return result;
	}

	void displayText(CCObject*) {
		FLAlertLayer::create("Level Rating", m_fields->textString, "OK")->show();
	}
};

// make work with compact level lists