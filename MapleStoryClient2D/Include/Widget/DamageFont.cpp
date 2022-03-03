
#include "DamageFont.h"
#include "Render/RenderManager.h"

CDamageFont::CDamageFont()
{
}

CDamageFont::CDamageFont(const CDamageFont& window) :
    CWidgetWindow(window)
{
    auto iter = window.m_ListDamage.begin();
    auto iterEnd = window.m_ListDamage.end();

    for (; iter != iterEnd; ++iter)
    {
        m_ListDamage.push_back((*iter));
    }
}

CDamageFont::~CDamageFont()
{
    auto iter = m_vecFileName.begin();
    auto iterEnd = m_vecFileName.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE_ARRAY((*iter));
    }

    auto Criticaliter = m_vecCriticalFileName.begin();
    auto CriticaliterEnd = m_vecCriticalFileName.end();

    for (; Criticaliter != CriticaliterEnd; ++Criticaliter)
    {
        SAFE_DELETE_ARRAY((*Criticaliter));
    }
}

void CDamageFont::Start()
{
}

bool CDamageFont::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/DamageFont/NoRed0.%d.png"), i);

        m_vecFileName.push_back(FileName);
    }

    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/DamageFont/NoCri0.%d.png"), i);

        m_vecCriticalFileName.push_back(FileName);
    }

    return true;
}

void CDamageFont::Update(float DeltaTime)
{

    m_Enable = true;

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

    auto Criticaliter = m_ListCriticalEffect.begin();
    auto CriticaliterEnd = m_ListCriticalEffect.end();

    for (; Criticaliter != CriticaliterEnd; ++Criticaliter)
    {
        Vector2 Pos = (*Criticaliter)->GetWidgetPos();
        (*Criticaliter)->SetPos(Pos.x, Pos.y + 17.f * DeltaTime);
        float Opacity = (*Criticaliter)->GetOpacity();
        (*Criticaliter)->SetOpacity(Opacity - DeltaTime / 1.5f);
    }

    CWidgetWindow::Update(DeltaTime);
}

void CDamageFont::PostUpdate(float DeltaTime)
{
    if (!m_Enable)
        return; 

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

    if (!m_ListCriticalEffect.empty())
    {
        auto iter = m_ListCriticalEffect.begin();
        auto iterEnd = m_ListCriticalEffect.end();

        // 점점 위로 올라가면서 소멸하게하기
        for (; iter != iterEnd; ++iter)
        {
            if ((*iter)->GetOpacity() <= 0.f)
            {
                (*iter)->Destroy();
                iter = m_ListCriticalEffect.erase(iter);
                iterEnd = m_ListCriticalEffect.end();

                if (iter == iterEnd)
                    break;
            }
        }
    }

}

void CDamageFont::Render()
{
  /*  if (CRenderManager::GetInst()->GetStartFadeIn())
        return;*/

    if (!m_Enable)
        return;

    CWidgetWindow::Render();
}

CDamageFont* CDamageFont::Clone()
{
	return new CDamageFont(*this);
}
