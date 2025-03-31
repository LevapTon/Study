import pytest
from main import get_html, get_coll_of_hrefs, create_csv_doc, get_first_href
import os
import allure  # Добавляем Allure

SAMPLE_HTML = '''
<html>
    <body>
        <a class="links" href="https://example.com/file1.pdf" target=_blank>file1.pdf</a>
        <a class="links" href="https://example.com/file2.pdf" target=_blank>file2.pdf</a>
        <a class="links" href="https://example.com/file3.txt" target=_blank>file3.txt</a>
    </body>
</html>
'''

@allure.title("Test get_html function")
@allure.description("Checks if get_html returns a string from mocked request")
def test_get_html(mocker):
    mock_response = mocker.Mock()
    mock_response.text = 'test content'
    mock_response.encoding = 'utf-8'
    mocker.patch('requests.get', return_value=mock_response)
    result = get_html('https://fakeurl.com')
    assert result == 'test content'
    assert isinstance(result, str)

@allure.title("Test get_coll_of_hrefs function")
def test_get_coll_of_hrefs():
    open_key = '<a class="links" href="'
    mid_key = '" target=_blank>'
    close_key = '</a>'
    get_coll_of_hrefs(open_key, mid_key, close_key, SAMPLE_HTML)
    with open('Files.csv', 'r', encoding='utf-8') as f:
        content = f.read()
        assert 'id,href,name' in content
        assert 'https://example.com/file1.pdf,file1.pdf' in content
        assert 'https://example.com/file2.pdf,file2.pdf' in content
        assert 'file3.txt' not in content

@allure.title("Test create_csv_doc function")
def test_create_csv_doc():
    links = ['https://example.com/file1.pdf', 'https://example.com/file2.pdf']
    names = ['file1.pdf', 'file2.pdf']
    create_csv_doc(links, names)
    with open('Files.csv', 'r', encoding='utf-8') as f:
        lines = f.readlines()
        assert lines[0].strip() == 'id,href,name'
        assert lines[1].strip() == '1,https://example.com/file1.pdf,file1.pdf'
        assert lines[2].strip() == '2,https://example.com/file2.pdf,file2.pdf'

@allure.title("Test get_first_href function")
def test_get_first_href():
    open_key = '<a class="links" href="'
    mid_key = '" target=_blank>'
    close_key = '</a>'
    pos = 0
    result = get_first_href(open_key, mid_key, close_key, SAMPLE_HTML, pos)
    assert len(result) == 3
    assert result[1] == 'https://example.com/file1.pdf'
    assert result[2] == 'file1.pdf'

@pytest.fixture(autouse=True)
def cleanup():
    yield
    if os.path.exists('Files.csv'):
        os.remove('Files.csv')