SELECT 
  list_aggregate(tau_pt, 'listagg', '|'),
  list_aggregate(tau_eta, 'listagg', '|'),
  list_aggregate(jet_pt, 'listagg', '|'),
  list_aggregate(jet_eta, 'listagg', '|'),
  list_aggregate(muon_pt, 'listagg', '|'),
  list_aggregate(muon_eta, 'listagg', '|'),
  list_aggregate(el_pt, 'listagg', '|'),
  list_aggregate(el_eta, 'listagg', '|'),
  list_aggregate(ph_pt, 'listagg', '|'),
  list_aggregate(ph_eta, 'listagg', '|')  
FROM singleMu;