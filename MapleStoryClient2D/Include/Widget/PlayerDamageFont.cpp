
#include "PlayerDamageFont.h"
#include "Render/RenderManager.h"

CPlayerDamageFont::CPlayerDamageFont()
{
}

CPlayerDamageFont::CPlayerDamageFont(const CPlayerDamageFont& window) :
    CWidgetWindow(window)
{
    auto iter = window.m_ListDamage.begin();
    auto iterEnd = window.m_ListDamage.end();

    for (; iter != iterEnd; ++iter)
    {
        m_ListDamage.push_back((*iter));
    }
}

CPlayerDamageFont::~CPlayerDamageFont()
{
    auto iter = m_vecFileName.begin();
    auto iterEnd = m_vecFileName.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE_ARRAY((*iter));
    }
}

void CPlayerDamageFont::Start()
{
}

bool CPlayerDamageFont::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/DamageFont/NoViolet0.%d.png"), i);

        m_vecFileName.push_back(FileName);
    }

    return true;
}

void CPlayerDamageFont::Update(float DeltaTime)
{
    auto iter = m_ListDamage.begin();
    auto iterEnd = m_ListDamage.end();

    // 점점 위로 올라가면서 소멸하게하기
    for (; iter != iterEnd; ++iter)
    {
        Vector2 Pos = (*iter)->GetWidgetPos();
        (*iter)->SetPos(Pos.x, Pos.y + 17.f * DeltaTime);
        float Opacity = (*iter)->GetOpacity();
        (*iter)->SetOpacity(Opacity - DeltaTime / 1.5f);
    }

    CWidgetWindow::Update(DeltaTime);
}

void CPlayerDamageFont::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);

    if (!m_ListDamage.empty())
    {
        auto iter = m_ListDamage.begin();
        auto iterEnd = m_ListDamage.end();

        // 점점 위로 올라가면서 소멸하게하기
        for (; iter != iterEnd; ++iter)
        {
            if ((*iter)->GetOpacity() <= 0.f)
            {
                (*iter)->Destroy();
                iter = m_ListDamage.erase(iter);
                iterEnd = m_ListDamage.end();

                if (iter == iterEnd)
                    break;
            }
        }
    }
}

void CPlayerDamageFont::Render()
{
  /*  if (CRenderManager::GetInst()->GetStartFadeIn())
        return;*/

    CWidgetWindow::Render();
}

CPlayerDamageFont* CPlayerDamageFont::Clone()
{
    return new CPlayerDamageFont(*this);
}
