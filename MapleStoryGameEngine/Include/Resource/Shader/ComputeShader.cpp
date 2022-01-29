
#include "ComputeShader.h"
#include "../../PathManager.h"
#include "../../Device.h"

CComputeShader::CComputeShader() :
    m_CS(nullptr),
    m_CSBlob(nullptr)
{
    m_Type = Shader_Type::Compute;
}

CComputeShader::~CComputeShader()
{
    SAFE_RELEASE(m_CS);
    SAFE_RELEASE(m_CSBlob);
}

void CComputeShader::SetShader()
{
    CDevice::GetInst()->GetContext()->CSSetShader(m_CS, nullptr, 0);
}

bool CComputeShader::LoadComputeShader(const char* EntryName, const TCHAR* FileName, const std::string& PathName)
{
    SAFE_RELEASE(m_CS);
    SAFE_RELEASE(m_CSBlob);

    unsigned int Flag = 0;

#ifdef _DEBUG
    Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

    if (Info)
        lstrcpy(FullPath, Info->Path);

    lstrcat(FullPath, FileName);

    ID3DBlob* Error = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        EntryName, "cs_5_0", Flag, 0, &m_CSBlob, &Error)))
    {
        OutputDebugStringA((char*)Error->GetBufferPointer());
        OutputDebugStringA("\n");

        return false;
    }

    // 컴파일된 코드를 이용해서 Shader를 만든다.
    if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(m_CSBlob->GetBufferPointer(),
        m_CSBlob->GetBufferSize(), nullptr, &m_CS)))
        return false;


    return true;
}

void CComputeShader::Excute(unsigned int x, unsigned int y, unsigned int z)
{
    // ComputeShader를 실행시켜준다.
    SetShader();
    // 스레드 그룹을 x * y * z개 만들어주고 스레드가 동작하기 시작한다. 만약에 Particle Update Shader에서
    // numthreads[64, 1, 1]로 64개의 그룹 스레드를 만들었고 여기서 Dispatch(3,2,1) 호출하면
    // 한 그룹당 64개의 스레드를 가진 그룹이 6(=3x2x1)개 만들어질 것이다
    // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-dispatch
    CDevice::GetInst()->GetContext()->Dispatch(x, y, z);
    //CDevice::GetInst()->GetContext()->CSSetShader(nullptr, nullptr, 0);
}