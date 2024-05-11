#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;
using namespace geode::utils::string;

class $modify(LevelCell) {
	struct Fields {
		CCSprite* ratingSprite;
		CCLabelBMFont* ratingLabel;
	};
	void setSpriteAndLabel(int number, cocos2d::ccColor3B color) {
		m_fields->ratingSprite = CCSprite::createWithSpriteFrameName(fmt::format("diffIcon_0{}_btn_001.png", number).c_str());
		m_fields->ratingLabel->setColor(color);
	}
	void loadCustomLevelCell() {
		LevelCell::loadCustomLevelCell();
		
		if (!Mod::get()->getSettingValue<bool>("levelcell")) return;
		
		CCLayer* mainLayer = static_cast<CCLayer*>(getChildByID("main-layer"));
		CCSprite* likeSprite = static_cast<CCSprite*>(mainLayer->getChildByID("likes-icon"));
		CCLabelBMFont* likeLabel = static_cast<CCLabelBMFont*>(mainLayer->getChildByID("likes-label"));
		CCNode* likeLabelEnding = reinterpret_cast<CCNode*>(likeLabel->getChildren()->objectAtIndex(likeLabel->getChildrenCount() - 1));

		double theDownloads = double(m_level->m_downloads);
		if (theDownloads < 1.0) theDownloads = 1.0;
		double ratingRatio = floor(((double(m_level->m_likes)/theDownloads)*100)*100+0.5)/100;
		
		m_fields->ratingLabel = CCLabelBMFont::create(fmt::format("{}%", ratingRatio).c_str(), "bigFont.fnt");

		if (ratingRatio >= 8) {
			setSpriteAndLabel(1, { 43, 255, 0 });
		}
		else if (ratingRatio >= 6) {
			setSpriteAndLabel(2, { 136, 255, 0 });
		}
		else if (ratingRatio >= 4) {
			setSpriteAndLabel(3, { 200, 255, 0 });
		}
		else if (ratingRatio >= 2) {
			setSpriteAndLabel(4, { 255, 196, 0 });
		}
		else if (ratingRatio > 0) {
			setSpriteAndLabel(5, { 255, 123, 0 });
		}
		else {
			setSpriteAndLabel(6, { 255, 0, 0 });
		}
		
		m_fields->ratingSprite->setScale(0.5);
		m_fields->ratingSprite->setID("ratings-icon"_spr);

		m_fields->ratingLabel->setScale(0.4);
		m_fields->ratingLabel->setID("ratings-label"_spr);

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
	struct Fields {
		CCLabelBMFont* ratingLabel;
		std::string textString;
	};
	void setColorAndString(cocos2d::ccColor3B color, const char* colorTag, double ratingRatio, const char* customStringOne, const char* customStringTwo) {
		m_fields->ratingLabel->setColor(color);
		m_fields->textString = fmt::format("Out of <cl>{} players</c>, <{}>{}%</c> {}\n\n{}", m_level->m_downloads, colorTag, ratingRatio, customStringOne, customStringTwo);
	}
	bool init(GJGameLevel* p0, bool p1) {
		bool result = LevelInfoLayer::init(p0, p1);
		
		if (!Mod::get()->getSettingValue<bool>("levelinfo")) {
			return result;
		}
		
		double theDownloads = double(m_level->m_downloads);
		if (theDownloads < 1.0) theDownloads = 1.0;
		double ratingRatio = floor(((double(m_level->m_likes)/theDownloads)*100)*100+0.5)/100;
		CCMenu* ratioMenu = CCMenu::create();
		CCLabelBMFont* downloadsLabel = static_cast<CCLabelBMFont*>(getChildByID("downloads-label"));

		m_fields->ratingLabel = CCLabelBMFont::create(fmt::format("({}%)", ratingRatio).c_str(), "bigFont.fnt");

		if (ratingRatio >= 8) {
			setColorAndString({ 43, 255, 0 }, "cg", ratingRatio, "have enjoyed it!" , "Assuming this level didn't beg for likes, this is probably an excellent level!");
		}
		else if (ratingRatio >= 6) {
			setColorAndString({ 136, 255, 0 }, "cg", ratingRatio, "may have liked it!", "A majority of players may have enjoyed playing this level!");
		}
		else if (ratingRatio >= 4) {
			setColorAndString({ 200, 255, 0 }, "cy", ratingRatio, "have somewhat liked it.", "The enjoyment is a bit of a mixed bag here.");
		}
		else if (ratingRatio >= 2) {
			setColorAndString({ 255, 196, 0 }, "co", ratingRatio, "have decided it's okay.", "You might be in for a bad time.");
		}
		else if (ratingRatio > 0) {
			setColorAndString({ 255, 123, 0 }, "co", ratingRatio, "have given it a like.", "Buckle up for lots of bugs, or get ready to struggle.");
		}
		else {
			if (ratingRatio != 0.0) ratingRatio *= -1;
			const char* magicString = "<cr>Nobody</c> has enjoyed this level.\nStart running.\nThis is probably a bad level.";
			// half the time when a rated level is in the negatives it's actually a good level :( -zmx
			if (m_level->m_levelID.value() == 104304230 || toLower(std::string(m_level->m_levelName)).find("dislike this") != std::string::npos) magicString = "This percentage is meaningless; the level's author literally told you to dislike it.";
			else if (m_level->m_stars.value() != 0) magicString = "It's possible this level was a victim of dislikebotting, or that the publisher of this level was caught in a recent controversy. In either case, that's not for me to say; I'm not a psychic.";
			setColorAndString({ 255, 0, 0 }, "cr", (ratingRatio), "have disliked it.", magicString);
		}

		ratioMenu->setPosition({downloadsLabel->getPositionX() + 23, downloadsLabel->getPositionY() - 14});
		m_fields->ratingLabel->setScale(0.4);
		m_fields->ratingLabel->setID("ratings-label"_spr);

		CCMenuItemSpriteExtra* ratioButton = CCMenuItemSpriteExtra::create(reinterpret_cast<CCSprite*>(m_fields->ratingLabel), this, menu_selector(ratioDisplay::displayText));
 		
 		ratioMenu->addChild(ratioButton);
		addChild(ratioMenu);

		return result;
	}

	void displayText(CCObject*) {
		FLAlertLayer::create("Level Rating", m_fields->textString, "OK")->show();
	}
};